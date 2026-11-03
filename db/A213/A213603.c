/*
 * A213603  Denominator of expected minimum number of yes-no questions
 *          required to determine the value of a card randomly selected from a
 *          deck consisting of one 1, two 2's, three 3's, ..., and n n's.
 * https://oeis.org/A213603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the expected minimum number of yes‑no
 *            questions needed to determine a random card from a deck with
 *            multiplicities 1,2,3,…,n. The expectation is computed by forming
 *            a Huffman tree on the weights 1 … n; let C(n) be the weighted
 *            path length (sum of weight × depth). Then a(n) =...
 * Range:     u8 n <= 11, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static void heapify_down(seq_t *heap, size_t size, size_t idx)
{
    while (1) {
        size_t left  = idx * 2 + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < size && heap[left] < heap[smallest])
            smallest = left;
        if (right < size && heap[right] < heap[smallest])
            smallest = right;
        if (smallest == idx)
            break;
        seq_t tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

static seq_t heap_pop(seq_t *heap, size_t *size)
{
    seq_t ret = heap[0];
    heap[0] = heap[--(*size)];
    if (*size > 0)
        heapify_down(heap, *size, 0);
    return ret;
}

static void heapify_up(seq_t *heap, size_t size, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent] <= heap[idx])
            break;
        seq_t tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static void heap_push(seq_t *heap, size_t *size, seq_t val)
{
    heap[*size] = val;
    heapify_up(heap, *size, *size);
    ++(*size);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* by convention, same as a(1) */

    /* Allocate heap with capacity n (enough to hold all items). */
    seq_t *heap = mem_new(seq_t, (size_t)n);
    if (heap == NULL)
        return 0;                /* out of memory */

    /* Initialise heap with weights 1 .. n. */
    for (idx_t i = 1; i <= n; ++i)
        heap[i - 1] = (seq_t)i;

    size_t hsize = (size_t)n;

    /* Build min‑heap */
    for (size_t i = hsize / 2; i > 0; --i)
        heapify_down(heap, hsize, i - 1);

    seq_t cost = 0;

    /* Huffman algorithm: merge until one node remains. */
    while (hsize > 1) {
        seq_t a = heap_pop(heap, &hsize);
        seq_t b = heap_pop(heap, &hsize);
        seq_t s = a + b;
        cost += s;
        heap_push(heap, &hsize, s);
    }

    mem_free(heap);

    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t g = gcd(cost, total);
    return total / g;
}
