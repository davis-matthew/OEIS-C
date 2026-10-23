/*
 * A126488  Number of base 20 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base 20 n‑digit strings with adjacent digits
 *            differing by at most 3. Computation: dynamic programming with a
 *            20‑state automaton. a(0) = 1, a(1) = 20, recurrence given by the
 *            adjacency matrix of the graph where |i-j|≤3. This implementation
 *            works for any sequence type seq_t and any...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base case: empty string */
    if (n == 0)
        return (seq_t)1;

    /* Number of digits in base 20 */
    const idx_t D = 20;

    /* counts for strings of the current length */
    seq_t cur[D];
    seq_t nxt[D];

    /* initial distribution: one string of length 1 ending in each digit */
    for (idx_t d = 0; d < D; ++d)
        cur[d] = (seq_t)1;

    /* Iterate through lengths 2 … n */
    for (idx_t step = 1; step < n; ++step) {
        for (idx_t d = 0; d < D; ++d) {
            seq_t sum = cur[d];               /* same digit */
            if (d >= 1) sum += cur[d-1];
            if (d >= 2) sum += cur[d-2];
            if (d >= 3) sum += cur[d-3];
            if (d + 1 < D) sum += cur[d+1];
            if (d + 2 < D) sum += cur[d+2];
            if (d + 3 < D) sum += cur[d+3];
            nxt[d] = sum;
        }
        /* swap cur and nxt for next iteration */
        for (idx_t d = 0; d < D; ++d)
            cur[d] = nxt[d];
    }

    /* Total number of strings of length n */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < D; ++d)
        total += cur[d];

    return total;
}
