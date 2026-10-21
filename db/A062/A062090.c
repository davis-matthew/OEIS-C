/*
 * A062090  a(1) = 1, a(n) = smallest odd number that does not divide the
 *          product of all previous terms.
 * https://oeis.org/A062090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=1, a(n)= smallest odd number that does not divide the *
 *            product of a(1)..a(n‑1). The sequence consists of all numbers of
 *            the * form p^(2^k) (p odd prime, k≥0) sorted increasingly. This
 *            implementation * generates the ordered list of such powers using
 *            a min‑heap. For each * odd prime p the hea...
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, cmp_seq_t, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/build_primes_upto.h"

typedef struct {
    seq_t prime;   /* odd prime p */
    seq_t value;   /* current value p^(2^e) */
} node_t;

static int cmp_node(const node_t *a, const node_t *b)
{
    return cmp_seq_t(&a->value, &b->value);
}

static void swap_node(node_t *a, node_t *b)
{
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_insert(node_t *heap, size_t *size, node_t item)
{
    size_t i = (*size)++;      /* place at end */
    heap[i] = item;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (cmp_node(&heap[i], &heap[parent]) < 0) {
            swap_node(&heap[i], &heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

static node_t heap_extract_min(node_t *heap, size_t *size)
{
    node_t min = heap[0];
    heap[0] = heap[--(*size)];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < *size && cmp_node(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right < *size && cmp_node(&heap[right], &heap[smallest]) < 0)
            smallest = right;
        if (smallest != i) {
            swap_node(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return min;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Find enough odd primes.  They are needed as heap entries because
     * each odd prime contributes one series of powers.  We request more
     * than n to be safe. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    idx_t limit = n < 1000 ? 1000 : n * 10;          /* initial guess */
    while (1) {
        build_primes_upto((idx_t)limit, &primes, &prime_cnt);
        size_t odd_cnt = prime_cnt - 1;              /* skip prime 2 */
        if (odd_cnt >= n) break;
        free(primes);
        limit <<= 1;                                 /* double limit */
    }

    /* Allocate heap of odd primes owned by the algorithm */
    size_t heap_cap = prime_cnt - 1;                 /* number of odd primes */
    node_t *heap = (node_t *)malloc(sizeof(node_t) * heap_cap);
    if (!heap) { free(primes); return (seq_t)0; }    /* allocation failure */

    /* Insert every odd prime into the heap with initial value p^1 */
    size_t heap_size = 0;
    for (size_t i = 1; i < prime_cnt; ++i) {         /* primes[0] == 2 */
        node_t node;
        node.prime = (seq_t)primes[i];
        node.value = node.prime;                     /* p^1 */
        heap_insert(heap, &heap_size, node);
    }

    /* Generate the sequence term-by-term */
    seq_t result = 0;
    for (idx_t idx = 2; idx <= n; ++idx) {
        node_t cur = heap_extract_min(heap, &heap_size);
        result = cur.value;
        if (idx == n) break;                        /* desired term found */

        /* Next power for this prime is square of current value */
        cur.value = seq_mul(cur.value, cur.value);
        heap_insert(heap, &heap_size, cur);
    }

    free(heap);
    free(primes);
    return result;
}
