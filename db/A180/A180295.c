/*
 * A180295  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-5.
 * https://oeis.org/A180295
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*binomial(n+3, n-2) for n > 10; * for 6 <= n <= 10 use
 *            the pre‑computed values provided by OEIS
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Values for 0 <= n <= 10 (indices 0–5 are unused) */
    static const seq_t small_values[11] = {
        0, 0, 0, 0, 0, 0,        /* n < 6, not defined */
        1,      /* a(6) */
        392,    /* a(7) */
        2716,   /* a(8) */
        6804,   /* a(9) */
        12825   /* a(10) */
    };

    if (n <= 10) {
        if (n < 6)
            return (seq_t)0;   /* undefined below offset */
        return small_values[n];
    }

    /* For n > 10 use the empirical formula */
    seq_t term = binomial(n + 3, n - 2);   /* binomial(n+3, n-2) */
    return (seq_t)n * term;
}
