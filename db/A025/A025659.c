/*
 * A025659  Exponent of 6 (value of j) in n-th number of form 5^i*6^j.
 * https://oeis.org/A025659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hash_insert
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A025659: Exponent of 6 in the n‑th number of the form 5^i * 6^j.
 * The algorithm uses a min‑heap ordered by the real logarithm of the numbers
 * (i*ln5 + j*ln6) so that we never overflow 64‑bit arithmetic.  Each
 * extracted heap element represents a number 5^i * 6^j and its j‑exponent
 * is the sequence value a(n).  To avoid generating duplicates, a simple
 * open‑addressing hash table remembers which (i,j) pairs have already
 * been pushed onto the heap.
 *
 * This approach runs in O(n log n) time and uses O(n) memory,
 * satisfying the performance requirement for n up to 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/hash_insert.h"

typedef struct {
    long double val;   /* real logarithm of 5^i * 6^j */
    idx_t i;
    idx_t j;
} node_t;

static inline void heap_push(node_t *heap, size_t *sz, node_t n)
{
    size_t idx = *sz;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= n.val) break;
        heap[idx] = heap[parent];
        idx = parent;
    }
    heap[idx] = n;
    ++(*sz);
}

static inline node_t heap_pop(node_t *heap, size_t *sz)
{
    node_t ret = heap[0];
    --(*sz);
    node_t last = heap[*sz];
    size_t idx = 0;
    while (true) {
        size_t left = (idx << 1) + 1;
        if (left >= *sz) break;
        size_t right = left + 1;
        size_t child = (right < *sz && heap[right].val < heap[left].val) ? right : left;
        if (last.val <= heap[child].val) break;
        heap[idx] = heap[child];
        idx = child;
    }
    heap[idx] = last;
    return ret;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, behaviour below 1 does not matter */

    /* Preallocate heap large enough for 2*n + 10 nodes */
    size_t heap_cap = (size_t)n * 2 + 16;
    node_t *heap = (node_t *)malloc(heap_cap * sizeof(node_t));
    size_t heap_sz = 0;

    /* Hash table size power of two, at least 4*n to keep load factor < 0.5 */
    size_t hash_cap = 1;
    while (hash_cap < (size_t)n * 4) hash_cap <<= 1;
    uint64_t *hash_keys = (uint64_t *)malloc(hash_cap * sizeof(uint64_t));
    uint8_t *hash_used  = (uint8_t *)calloc(hash_cap, 1);

    /* Logarithms for 5 and 6 */
    const long double ln5 = logl((long double)5);
    const long double ln6 = logl((long double)6);

    /* Start with (i,j) = (0,0) */
    node_t start = { .val = 0.0L, .i = 0, .j = 0 };
    uint64_t start_key = ((uint64_t)0 << 32) | 0;
    hash_keys[0] = start_key;
    hash_used[0] = 1;  /* mark visited */
    heap_push(heap, &heap_sz, start);

    idx_t count = 0;
    seq_t last_j = 0;

    while (count < n) {
        node_t cur = heap_pop(heap, &heap_sz);
        ++count;
        last_j = (seq_t)cur.j;

        /* generate child (i+1,j) */
        idx_t ni = cur.i + 1;
        uint64_t key1 = ((uint64_t)ni << 32) | (uint64_t)cur.j;
        if (!hash_insert(hash_keys, hash_used, hash_cap, key1)) {
            node_t child = { .val = cur.val + ln5, .i = ni, .j = cur.j };
            heap_push(heap, &heap_sz, child);
        }

        /* generate child (i,j+1) */
        idx_t nj = cur.j + 1;
        uint64_t key2 = ((uint64_t)cur.i << 32) | (uint64_t)nj;
        if (!hash_insert(hash_keys, hash_used, hash_cap, key2)) {
            node_t child = { .val = cur.val + ln6, .i = cur.i, .j = nj };
            heap_push(heap, &heap_sz, child);
        }
    }

    free(heap);
    free(hash_keys);
    free(hash_used);

    return last_j;
}
