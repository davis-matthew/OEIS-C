/*
 * A180002  Place a(n) red and b(n) blue balls in an urn; draw 5 balls without
 *          replacement; Probability(5 red balls) = Probability(3 red and 2
 *          blue balls).
 * https://oeis.org/A180002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180002: a(n+9) = 39*a(n+6) - 39*a(n+3) + a(n)
 * with a(1)=4, a(2)=8, a(3)=24, a(4)=43, a(5)=179, a(6)=783,
 * a(7)=1504, a(8)=6668, a(9)=29604. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* sequence is 1‑based; guard against 0 */
    static const seq_t init[9] = {
        4, 8, 24, 43, 179, 783, 1504, 6668, 29604
    };
    if (n <= 9) return init[n-1];

    /* Allocate space for the values up to n */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) abort();          /* out of memory */
    for (idx_t i = 1; i <= 9; ++i) seq[i] = init[i-1];

    /* Compute terms using the linear recurrence */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t t1 = seq[i-3] * 39;   /* 39 * a(i-3) */
        seq_t t2 = seq[i-6] * 39;   /* 39 * a(i-6) */
        seq[i] = t1 - t2 + seq[i-9];/* a(i) = 39*a(i-3) - 39*a(i-6) + a(i-9) */
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
