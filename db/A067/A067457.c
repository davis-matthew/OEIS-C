/*
 * A067457  Smallest n-th power starting with n.
 * https://oeis.org/A067457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest m = a^n such that the decimal representation *
 *            of m starts with the digits of n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/digits_idx.h"

/* Fast exponentiation of a^e (e >= 1), result in seq_t. */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
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
    if (n == 1)
        return (seq_t)1;                /* 1^1 = 1 */

    const idx_t L = digits_idx(n);     /* digits of n */

    for (uint64_t a = 2;; ++a)
    {
        /* Compute fractional part of n * log10(a). */
        long double loga = log10l((long double)a);
        long double f = n * loga;
        long double intpart;
        f = modfl(f, &intpart);

        /* Leading digits of a^n. */
        long double leading = powl(10.0L, f + (long double)(L - 1));
        idx_t leading_int = (idx_t)(floorl(leading + 1e-12L));

        if (leading_int == n)
        {
            seq_t base = (seq_t)a;
            return pow_seq_local(base, n);   /* a^n */
        }
    }
}
