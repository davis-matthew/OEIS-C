/*
 * A057668  Number of minimal 7-covers of a labeled n-set.
 * https://oeis.org/A057668
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (127^n - 7*126^n + 21*125^n - 35*124^n + 35*123^n * -
 *            21*122^n + 7*121^n - 120^n) / 7! with a(7)=1
 * Range:     u8 none, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

static inline unsigned __int128 pow_uint128(uint64_t base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;                    /* Undefined below offset */

    /* 7! = 5040 */
    const seq_t denom = (seq_t)5040;

    /* For 64‑bit seq_t we use 128‑bit arithmetic to avoid overflow. */
    if (sizeof(seq_t) <= 8)          /* uint8_t … uint64_t */
    {
        unsigned __int128 term127 = pow_uint128(127u, n);
        unsigned __int128 term126 = pow_uint128(126u, n);
        unsigned __int128 term125 = pow_uint128(125u, n);
        unsigned __int128 term124 = pow_uint128(124u, n);
        unsigned __int128 term123 = pow_uint128(123u, n);
        unsigned __int128 term122 = pow_uint128(122u, n);
        unsigned __int128 term121 = pow_uint128(121u, n);
        unsigned __int128 term120 = pow_uint128(120u, n);

        unsigned __int128 sum =
              term127
            - 7u * term126
            + 21u * term125
            - 35u * term124
            + 35u * term123
            - 21u * term122
            + 7u * term121
            - term120;

        /* Exact division because the result is an integer */
        return (seq_t)(sum / denom);
    }

    /* For arbitrary‑precision seq_t we rely on the utilities. */
    seq_t sum_pos = big_pow((seq_t)127, (seq_t)n);
    seq_t t = big_pow((seq_t)125, (seq_t)n); sum_pos += mul_small(t, 21);
    t = big_pow((seq_t)123, (seq_t)n);        sum_pos += mul_small(t, 35);
    t = big_pow((seq_t)121, (seq_t)n);        sum_pos += mul_small(t, 7);

    seq_t sum_neg = big_pow((seq_t)126, (seq_t)n);
    sum_neg = mul_small(sum_neg, 7);
    t = big_pow((seq_t)124, (seq_t)n);        sum_neg += mul_small(t, 35);
    t = big_pow((seq_t)122, (seq_t)n);        sum_neg += mul_small(t, 21);
    t = big_pow((seq_t)120, (seq_t)n);        sum_neg += t;

    seq_t total = sum_pos - sum_neg;          /* guaranteed non‑negative */
    return total / denom;
}
