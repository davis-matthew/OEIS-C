/*
 * A189162  The maximum possible value for the apex of a triangle of numbers
 *          whose base consists of a permutation of the numbers 1 to n, and
 *          each number in a higher row is the sum of the two numbers directly
 *          below it.
 * https://oeis.org/A189162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = Σ_{k=1..n} b_k · C(n‑1, k‑1) * where * b_k = 2k‑1 (for
 *            k ≤ ceil(n/2)) * b_k = 2(n‑k+1) (for k > ceil(n/2)) * C(n‑1,
 *            k‑1) is the binomial coefficient. * This implements the optimal
 *            base permutation described in the OEIS * entry and works for any
 *            sufficiently large integer type seq_t
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* guard for accidental use of index 0; not part of the sequence */
    if (n == 0) return 0;

    seq_t sum   = 0;   /* running sum for a(n) */
    seq_t comb  = 1;   /* current binomial coefficient C(n-1, j) */
    idx_t half  = (n + 1) / 2;   /* ceil(n/2) */

    for (idx_t j = 0; j < n; ++j) {
        seq_t base;
        if (j < half)                 /* odd increasing part */
            base = 2 * (seq_t)j + 1;
        else                          /* even decreasing part */
            base = 2 * (seq_t)(n - j);

        sum += base * comb;

        /* update binomial coefficient for next j, except after the last term */
        if (j + 1 < n)
            comb = comb * (seq_t)(n - 1 - j) / (seq_t)(j + 1);
    }
    return sum;
}
