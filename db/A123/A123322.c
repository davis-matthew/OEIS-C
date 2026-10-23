/*
 * A123322  Products of 8 distinct primes (squarefree 8-almost primes).
 * https://oeis.org/A123322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9566, u64 n <= 9566, u128 n <= 9566,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: products of 8 distinct primes (squarefree 8‑almost primes) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------------------------------------------------------------- */
/* generic capacity helper                                         */
static void *ensure_capacity(void *old, size_t elem_size,
                             size_t *cap, size_t needed)
{
    if (needed <= *cap) return old;
    size_t new_cap = (*cap == 0) ? 256 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    return realloc(old, new_cap * elem_size);
}

/* --------------------------------------------------------------- */
/* prime sieve                                                      */
#define PRIME_LIMIT 200000           /* enough for n up to ~10000      */
static uint32_t *primes = NULL;
static size_t prime_count = 0;

static void sieve_primes(void)
{
    bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            ++cnt;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            primes[idx++] = (uint32_t)i;

    prime_count = cnt;
    free(is_composite);
}

/* --------------------------------------------------------------- */
/* heap node: product and 8 increasing indices                     */
struct Node {
    seq_t prod;
    uint16_t idx[8];
};

static struct Node *heap = NULL;
static size_t heap_cnt = 0, heap_cap = 0;

static void heap_push(struct Node node)
{
    heap = (struct Node *)ensure_capacity(heap, sizeof(struct Node),
                                          &heap_cap, heap_cnt + 1);
    size_t i = heap_cnt++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].prod <= heap[i].prod) break;
        struct Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static struct Node heap_pop(void)
{
    struct Node res = heap[0];
    heap[0] = heap[--heap_cnt];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_cnt) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < heap_cnt && heap[r].prod < heap[l].prod)
            smallest = r;
        if (heap[smallest].prod >= heap[i].prod) break;
        struct Node tmp = heap[smallest];
        heap[smallest] = heap[i];
        heap[i] = tmp;
        i = smallest;
    }
    return res;
}

/* --------------------------------------------------------------- */
/* cache of computed terms                                          */
static seq_t *cache = NULL;
static size_t cache_cnt = 0, cache_cap = 0;

/* --------------------------------------------------------------- */
/* set of visited products                                         */
static seq_t *visited = NULL;
static size_t visited_cnt = 0, visited_cap = 0;

static bool visited_contains(seq_t val)
{
    for (size_t i = 0; i < visited_cnt; ++i)
        if (visited[i] == val) return true;
    return false;
}

static void visited_add(seq_t val)
{
    visited = (seq_t *)ensure_capacity(visited, sizeof(seq_t),
                                      &visited_cap, visited_cnt + 1);
    visited[visited_cnt++] = val;
}

/* --------------------------------------------------------------- */
/* initialization                                                  */
static bool initialized = false;

static void init_static(void)
{
    sieve_primes();

    struct Node root;
    for (size_t i = 0; i < 8; ++i)
        root.idx[i] = (uint16_t)i;
    seq_t prod = 1;
    for (size_t i = 0; i < 8; ++i)
        prod *= (seq_t)primes[root.idx[i]];
    root.prod = prod;

    heap = NULL;
    heap_cnt = 0;
    heap_cap = 0;
    heap_push(root);

    visited = NULL;
    visited_cnt = 0;
    visited_cap = 0;
    visited_add(prod);

    cache = NULL;
    cache_cnt = 0;
    cache_cap = 0;

    initialized = true;
}

/* --------------------------------------------------------------- */
/* main sequence function                                          */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 (offset is 1) */

    if (!initialized) init_static();

    size_t target = (size_t)n;

    while (cache_cnt < target) {
        struct Node cur = heap_pop();
        cache = (seq_t *)ensure_capacity(cache, sizeof(seq_t),
                                         &cache_cap, cache_cnt + 1);
        cache[cache_cnt++] = cur.prod;

        /* generate successors */
        for (size_t pos = 0; pos < 8; ++pos) {
            uint16_t old = cur.idx[pos];
            if (old + 1 + (7 - pos) >= prime_count) continue;

            struct Node child;
            for (size_t i = 0; i < pos; ++i)
                child.idx[i] = cur.idx[i];
            child.idx[pos] = old + 1;
            for (size_t j = pos + 1; j < 8; ++j)
                child.idx[j] = child.idx[j - 1] + 1;

            seq_t prod = 1;
            for (size_t t = 0; t < 8; ++t)
                prod *= (seq_t)primes[child.idx[t]];
            child.prod = prod;

            if (!visited_contains(child.prod)) {
                visited_add(child.prod);
                heap_push(child);
            }
        }
    }

    return cache[n - 1];
}
