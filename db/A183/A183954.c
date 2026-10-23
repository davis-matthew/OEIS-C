/*
 * A183954  Number of strings of numbers x(i=1..3) in 0..n with sum i^2*x(i)
 *          equal to n*9.
 * https://oeis.org/A183954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x1,x2,x3) with 0 ≤ xi ≤ n satisfying
 *            x1 + 4·x2 + 9·x3 = 9·n. Computation: iterate over x3, determine
 *            the feasible interval for x2 from 8n ≤ 4x2 + 9x3 ≤ 9n, and count
 *            the integer solutions. O(n) time
 * Range:     u8 n <= 46, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term; below that behaviour is irrelevant */
    if (n == 0) return 0;

    seq_t count = 0;

    /* use signed 64‑bit intermediates; for n up to 10⁶ this is safe */
    const long long nL = (long long)n;

    for (idx_t x3 = 0; x3 <= n; ++x3) {
        /* numerator for lowest x2: ceil((8n – 9x3)/4) */
        long long numL = 8 * nL - 9 * (long long)x3;
        long long L;
        if (numL >= 0) {
            L = (numL + 3) / 4;          /* ceil division for non‑negative */
        } else {
            L = numL / 4;                /* for negative, truncation gives ceil */
        }

        /* numerator for highest x2: floor((9n – 9x3)/4) */
        long long numR = 9 * nL - 9 * (long long)x3;   /* always non‑negative */
        long long R = numR / 4;                 /* floor division */

        if (L < 0) L = 0;
        if (R > (long long)n) R = (long long)n;

        if (L <= R) {
            /* number of admissible x2 values for this x3 */
            count += (seq_t)(R - L + 1);
        }
    }

    return count;
}
