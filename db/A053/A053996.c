/*
 * A053996  Number of bases of n-dimensional vector space over GF(4).
 * https://oeis.org/A053996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n - 1)(4^n - 4)...(4^n - 4^(n-1)) / n! (n >= 0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t pow_seq_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    const seq_t four = (seq_t)4;
    seq_t pow4_n = pow_seq_uint(four, n);

    seq_t res = 1;
    seq_t pow4_k = 1;               /* 4^0 */

    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t term = pow4_n - pow4_k;            /* 4^n - 4^(j-1) */
        seq_t denom = (seq_t)j;

        seq_t g = gcd(term, denom);
        term /= g;
        denom /= g;

        res = res * term;
        if (denom != 1)
            res = res / denom;                  /* division is exact */

        pow4_k = pow4_k * four;                 /* advance 4^k */
    }

    return res;
}
