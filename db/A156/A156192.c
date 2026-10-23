/*
 * A156192  Denominator of Euler(n, 1/7).
 * https://oeis.org/A156192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 7^n * 2^{v2(n+1)}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  v2(k) = exponent of 2 dividing k          */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* Count trailing zeros in the binary representation of x.  */
static idx_t _v2(idx_t x)
{
    idx_t v = 0;
    while ((x & 1ULL) == 0)
    {
        ++v;
        x >>= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* 7^n */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);

    /* 2^{v2(n+1)} */
    idx_t v = _v2(n + 1);              /* v2(n+1) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)v);

    /* product */
    return seq_mul(pow7, pow2);
}
