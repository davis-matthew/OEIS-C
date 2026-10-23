/*
 * A104705  First terms in the rearrangements of integer numbers (see
 *          comments).
 * https://oeis.org/A104705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A104705
    Formula: a(n) is the n‑th removed element when repeatedly
    taking the first element of the infinite list of natural numbers
    and inserting it prime[value] positions to the right.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------  implicit treap that stores the current list  ---------- */
struct Node {
    uint64_t val;     /* value stored in the node                    */
    uint32_t prio;    /* random priority for the heap property       */
    struct Node *l, *r;
    size_t sz;        /* size of the subtree rooted at this node     */
};

static inline size_t node_sz(struct Node *t) { return t ? t->sz : 0; }

static void upd(struct Node *t)
{
    if (t) t->sz = 1 + node_sz(t->l) + node_sz(t->r);
}

/* random generator for treap priorities */
static uint32_t rng_state = 2463534242;
static inline uint32_t rand_uint32(void)
{
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state;
}

static struct Node *new_node(uint64_t v)
{
    struct Node *n = (struct Node *)malloc(sizeof *n);
    n->val = v;
    n->prio = rand_uint32();
    n->l = n->r = NULL;
    n->sz = 1;
    return n;
}

static struct Node *treap_merge(struct Node *a, struct Node *b)
{
    if (!a) return b;
    if (!b) return a;
    if (a->prio < b->prio) {
        a->r = treap_merge(a->r, b);
        upd(a);
        return a;
    } else {
        b->l = treap_merge(a, b->l);
        upd(b);
        return b;
    }
}

static void treap_split(struct Node *t, size_t k,
                        struct Node **l, struct Node **r)
{
    if (!t) { *l = *r = NULL; return; }
    if (node_sz(t->l) >= k) {
        treap_split(t->l, k, l, &t->l);
        *r = t;
        upd(t);
    } else {
        treap_split(t->r, k - node_sz(t->l) - 1, &t->r, r);
        *l = t;
        upd(t);
    }
}

/* build a balanced treap that contains consecutive values [lo,hi] */
static struct Node *build_range(uint64_t lo, uint64_t hi)
{
    if (lo > hi) return NULL;
    uint64_t mid = (lo + hi) >> 1;
    struct Node *node = new_node(mid);
    node->l = build_range(lo, mid - 1);
    node->r = build_range(mid + 1, hi);
    upd(node);
    return node;
}

/* ----------  prime generation (5000000th prime > 86 million) ---------- */
#define PRIME_LIMIT 5000000UL          /* ample for all n up to 10000 */
static uint32_t *prime_list = NULL;
static size_t prime_count  = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    size_t limit = PRIME_LIMIT;
    bool *is_composite = (bool *)malloc(limit + 1);
    memset(is_composite, 0, limit + 1);
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof *prime_list);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    free(is_composite);
}

static inline uint32_t get_prime(uint64_t idx)   /* 1‑based */
{
    return prime_list[idx - 1];
}

/* ----------  memoisation of a(n) ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void cache_ensure(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------  main treap state ---------- */
static struct Node *root = NULL;
static uint64_t next_unseen = 2;   /* next natural number not yet in the treap */
static bool initialized = false;

/* ensure treap and primes are ready */
static void init_state(void)
{
    if (initialized) return;
    init_primes();

    root = new_node(1);
    next_unseen = 2;

    cache = (seq_t *)malloc(256 * sizeof(seq_t));
    cache_cap = 256;
    cache_len = 0;

    initialized = true;
}

/* perform one simulation step and record the removed value */
static void step(void)
{
    struct Node *first, *rest;
    treap_split(root, 1, &first, &rest);   /* split off the first node */
    root = rest;                            /* remaining list */

    uint64_t val = first->val;

    /* insertion position (0‑based) is prime[val] */
    size_t pos = get_prime(val);

    /* If necessary, extend the list with unseen natural numbers */
    while (pos > node_sz(rest)) {
        size_t need = pos + 1 - node_sz(rest);
        struct Node *extra = build_range(next_unseen, next_unseen + need - 1);
        next_unseen += need;
        rest = treap_merge(rest, extra);
    }

    /* split to insert at position pos */
    struct Node *left, *right;
    treap_split(rest, pos, &left, &right);

    first->l = first->r = NULL;
    upd(first);

    root = treap_merge(treap_merge(left, first), right);

    /* cache the removed value */
    cache_ensure(cache_len + 1);
    cache[cache_len++] = (seq_t)val;
}

/* ----------  public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, harmless          */
    init_state();

    while (cache_len < (size_t)n) step();

    return cache[n - 1];
}
