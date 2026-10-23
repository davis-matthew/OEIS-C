/*
 * A133068  Number of surjections from an n-element set to an eight-element
 *          set.
 * https://oeis.org/A133068
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..8} ((-1)^(8-k)*binomial(8,k)*k^n)
 * Range:     u8 none, u16 n <= 8, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t binom8k[9] = {1, 8, 28, 56, 70, 56, 28, 8, 1};

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 8)
        return 0; /* values below the offset are irrelevant */

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t k = 1; k <= 8; ++k)
    {
        seq_t term = binom8k[k] * pow_seq((seq_t)k, n);
        if ((8 - k) & 1)
            neg += term; /* negative sign */
        else
            pos += term; /* positive sign */
    }

    return pos - neg;
}
