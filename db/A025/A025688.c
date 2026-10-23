/*
 * A025688  Exponent of 10 (value of j) in n-th number of form 6^i*10^j.
 * https://oeis.org/A025688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 10 in the n‑th smallest number of the form
 *            6^i·10^j
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- heap node ---------- */
typedef struct {
    double val;          /* logarithmic value for ordering */
    unsigned int i;      /* exponent of 6 (non‑negative)  */
    unsigned int j;      /* exponent of 10 (non‑negative) */
} node_t;

/* ---------- binary heap ---------- */
typedef struct {
    node_t *data;   /* dynamic array of heap elements */
    size_t  len;    /* number of elements in the heap  */
    size_t  cap;    /* capacity of the array          */
} heap_t;

static void heap_init(heap_t *h)
{
    h->data = NULL;
    h->len  = 0;
    h->cap  = 0;
}

static void heap_push(heap_t *h, node_t const *x)
{
    if (h->len == h->cap) {
        size_t newcap = h->cap == 0 ? 16 : h->cap << 1;
        node_t *tmp = (node_t *)realloc(h->data, newcap * sizeof(node_t));
        if (!tmp) abort();          /* irrevocably unrecoverable */
        h->data = tmp;
        h->cap  = newcap;
    }
    size_t pos = h->len++;
    h->data[pos] = *x;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (h->data[parent].val <= h->data[pos].val) break;
        node_t tmp = h->data[parent];
        h->data[parent] = h->data[pos];
        h->data[pos] = tmp;
        pos = parent;
    }
}

static node_t heap_pop(heap_t *h)
{
    node_t top = h->data[0];
    --h->len;
    h->data[0] = h->data[h->len];
    size_t pos = 0;
    while (1) {
        size_t left  = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < h->len && h->data[left].val < h->data[smallest].val)
            smallest = left;
        if (right < h->len && h->data[right].val < h->data[smallest].val)
            smallest = right;
        if (smallest == pos) break;
        node_t tmp = h->data[pos];
        h->data[pos] = h->data[smallest];
        h->data[smallest] = tmp;
        pos = smallest;
    }
    return top;
}

/* ---------- hash set for visited (i, j) pairs ---------- */
typedef struct {
    uint64_t *tab;   /* encoded key ((i+1)<<32)|(j+1) */
    size_t   cap;    /* power of two */
    size_t   cnt;    /* number of used slots */
} hash_t;

static void hash_init(hash_t *h)
{
    h->cap = 16;
    h->cnt = 0;
    h->tab = (uint64_t *)calloc(h->cap, sizeof(uint64_t));
    if (!h->tab) abort();
}

static void hash_resize(hash_t *h)
{
    size_t newcap = h->cap << 1;
    uint64_t *newtab = (uint64_t *)calloc(newcap, sizeof(uint64_t));
    if (!newtab) abort();
    for (size_t i = 0; i < h->cap; ++i) {
        uint64_t k = h->tab[i];
        if (k == 0) continue;
        size_t pos = k & (newcap - 1);
        while (newtab[pos] != 0)
            pos = (pos + 1) & (newcap - 1);
        newtab[pos] = k;
    }
    free(h->tab);
    h->tab = newtab;
    h->cap = newcap;
}

/* insert key, return 1 if new, 0 if already present */
static int hash_insert(hash_t *h, unsigned int i, unsigned int j)
{
    if (h->cnt * 2 >= h->cap)
        hash_resize(h);
    uint64_t key = (((uint64_t)(i + 1)) << 32) | (uint64_t)(j + 1);
    size_t pos = key & (h->cap - 1);
    while (h->tab[pos] != 0) {
        if (h->tab[pos] == key)
            return 0;                /* already stored */
        pos = (pos + 1) & (h->cap - 1);
    }
    h->tab[pos] = key;
    ++h->cnt;
    return 1;                        /* inserted new */
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* for completeness, though n ≥ 1 */

    heap_t h;
    heap_init(&h);

    hash_t vis;
    hash_init(&vis);

    const double LOG6  = 1.791759469228055; /* ln 6  */
    const double LOG10 = 2.302585092994046; /* ln 10 */

    node_t root = { .val = 0.0, .i = 0, .j = 0 };
    heap_push(&h, &root);
    hash_insert(&vis, 0, 0);

    seq_t result = 0;
    for (size_t cnt = 0; cnt < (size_t)n; ++cnt) {
        node_t cur = heap_pop(&h);
        result = (seq_t)cur.j;              /* exponent of 10 */

        /* generate children */
        unsigned int i1 = cur.i + 1, j1 = cur.j;
        if (hash_insert(&vis, i1, j1)) {
            node_t child1 = { .val = cur.val + LOG6, .i = i1, .j = j1 };
            heap_push(&h, &child1);
        }
        unsigned int i2 = cur.i, j2 = cur.j + 1;
        if (hash_insert(&vis, i2, j2)) {
            node_t child2 = { .val = cur.val + LOG10, .i = i2, .j = j2 };
            heap_push(&h, &child2);
        }
    }

    free(h.data);
    free(vis.tab);
    return result;
}
