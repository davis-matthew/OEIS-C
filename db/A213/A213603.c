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
 * Helpers:   gcd, heap_pop, heap_push, heapify_down, heapify_up
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/heap_pop.h"
#include "../utils/heap_push.h"
#include "../utils/heapify_down.h"
#include "../utils/heapify_up.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* by convention, same as a(1) */

    /* Allocate heap with capacity n (enough to hold all items). */
    seq_t *heap = (seq_t *)malloc((size_t)n * sizeof(seq_t));
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

    free(heap);

    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t g = gcd(cost, total);
    return total / g;
}
