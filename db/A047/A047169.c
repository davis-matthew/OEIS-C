/*
 * A047169  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= n/2.
 * https://oeis.org/A047169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} binomial(⌊n/2⌋, k) * binomial(⌈n/2⌉, 5k)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 48, u64 n <= 91, u128 n <= 176,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* floor(n/2) and ceil(n/2) */
    idx_t fl = n / 2;          /* ⌊n/2⌋ */
    idx_t ce = n - fl;         /* ⌈n/2⌉ */

    /* maximum k such that k ≤ ⌊n/2⌋ and 5k ≤ ⌈n/2⌉ */
    idx_t kmax = fl;
    idx_t ce5  = ce / 5;
    if (ce5 < kmax)
        kmax = ce5;

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t left  = binom(fl, k);          /* binomial(⌊n/2⌋, k)   */
        seq_t right = binom(ce, 5 * k);      /* binomial(⌈n/2⌉, 5k)  */
        sum += left * right;                 /* exact multiplication for arbitrary‑precision builds */
    }
    return sum;
}
