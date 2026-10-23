/*
 * A119263  A permutation of the positive integers made by (alternatingly)
 *          reversing the order of the squarefree positive integers within
 *          each run of squarefree integers and reversing the order of the
 *          nonsquarefree positive integers within each run of nonsquarefree
 *          integers.
 * https://oeis.org/A119263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s + e - n * where s is the smallest integer ≤ n in the
 *            block containing n, and * e is the largest integer in that same
 *            block. A block consists of * consecutive integers that are all
 *            squarefree or all nonsquarefree. * Reversing each block yields a
 *            permutation; the n‑th term is obtained * by...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* validity guard – n≥1 in the sequence */

    seq_t x = (seq_t)n;
    seq_t start = x;

    /* find the start of the run containing n */
    while (start > 1) {
        seq_t prev = start - 1;
        if (is_squarefree(prev) == is_squarefree(start))
            start = prev;
        else
            break;
    }

    /* find the end of the run */
    seq_t end = start;
    while (true) {
        seq_t next = end + 1;
        if (is_squarefree(next) == is_squarefree(end))
            end = next;
        else
            break;
    }

    return start + end - x;
}
