/*
 * A124405  a(n) = 1 + Sum_{i=1..n} Sum_{j=1..n} i^j.
 * https://oeis.org/A124405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 + Sum_{j=2..n} j * (j^n - 1) / (j-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Basic check – OEIS offset starts at n = 1. */
    if (n == 0) {
        /* Not defined for n = 0; return 0 as a safe fallback. */
        return (seq_t)0;
    }

    seq_t result = (seq_t)n + 1;          /* start with n + 1         */
    seq_t sn     = (seq_t)n;              /* cast the exponent once    */

    for (idx_t j = 2; j <= n; ++j) {
        seq_t sj          = (seq_t)j;     /* base of the power        */
        seq_t pow_jn      = big_pow(sj, sn);      /* sj^n */

        seq_t numerator   = sj * (pow_jn - (seq_t)1);
        seq_t denominator = sj - (seq_t)1;
        seq_t term        = numerator / denominator;  /* integer division */

        result += term;
    }

    return result;
}
