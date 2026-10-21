/*
 * A059054  Integers which can be written as (b^k+1)/(b+1) for positive
 *          integers b and k.
 * https://oeis.org/A059054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059054
 *  Formula: a(n) is the nth smallest integer that can be written as (b^k+1)/(b+1)
 *  for positive integers b and k.
 *
 *  The implementation generates the sequence on demand using a merge of the
 *  increasing sequences for each odd exponent k (k ≥ 3).  The values for k≥3
 *  are appended to a min‑heap; the smallest value is popped, deduplicated,
 *  stored in a cache, and the next value for that exponent is inserted
 *  back into the heap.  The first term a(1)=1 is handled separately.
 *
 *  Only integers up to the last term produced are calculated; values that
 *  would overflow the native seq_t type are avoided by limiting the maximum
 *  exponent to 63 (the largest odd k with 2^k < 2^64).  This is sufficient
 *  for generating the first 10 000 terms with 64‑bit or greater seq_t
 *  representations.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

typedef struct {
    seq_t  v;   /* value (b^k+1)/(b+1) */
    idx_t  k;   /* exponent (odd, k>=3) */
    idx_t  b;   /* base       (b>=2) */
} Node;

/* ---------- heap implementation ---------- */

static Node *heap = NULL;
static size_t heap_sz = 0, heap_cap = 0;

static inline void heap_swap(size_t i, size_t j)
{
    Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static inline void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].v <= heap[i].v) break;
        heap_swap(p, i);
        i = p;
    }
}

static inline void heap_sift_down(size_t i)
{
    while (true) {
        size_t l = (i << 1) + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t c = (r < heap_sz && heap[r].v < heap[l].v) ? r : l;
        if (heap[i].v <= heap[c].v) break;
        heap_swap(i, c);
        i = c;
    }
}

static inline void heap_push(seq_t v, idx_t k, idx_t b)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 32;
        Node *newp = (Node *)realloc(heap, new_cap * sizeof(Node));
        if (!newp) abort();
        heap = newp;
        heap_cap = new_cap;
    }
    heap[heap_sz].v = v;
    heap[heap_sz].k = k;
    heap[heap_sz].b = b;
    heap_sift_up(heap_sz);
    ++heap_sz;
}

static inline Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_sz];
    heap_sift_down(0);
    return top;
}

/* ---------- cache of sequence values ---------- */

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static inline void cache_push(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* ---------- helper to compute (b^k+1)/(b+1) ---------- */

static inline seq_t compute_value(idx_t b, idx_t k)
{
    seq_t pow = big_pow((seq_t)b, (seq_t)k);
    return (pow + 1) / ((seq_t)(b + 1));
}

/* ---------- initialise data structures (once) ---------- */

static void init_once(void)
{
    /* a(1) = 1 */
    cache_push((seq_t)1);

    /* Seed heap with the first value (b=2) for each odd k >= 3 */
    const idx_t MAX_K = 63;           /* largest odd k with 2^k < 2^64 */
    for (idx_t k = 3; k <= MAX_K; k += 2) {
        seq_t val = compute_value((idx_t)2, k);
        heap_push(val, k, (idx_t)2);
    }
}

/* ---------- public sequence function ---------- */

seq_t a(idx_t n)
{
    static bool initialised = false;
    if (!initialised) {
        init_once();
        initialised = true;
    }

    /* Generate terms until we have at least n values */
    while (cache_len < n) {
        Node node = heap_pop();          /* smallest remaining value */

        /* deduplicate: only store if different from last unique term */
        if (node.v != cache[cache_len - 1]) {
            cache_push(node.v);
        }

        /* Push the next value for the same exponent k */
        idx_t nb = node.b + 1;
        seq_t next_val = compute_value(nb, node.k);
        heap_push(next_val, node.k, nb);
    }

    return cache[n - 1];
}
