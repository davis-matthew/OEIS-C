/*
 * A021121  Decimal expansion of 1/117.
 * https://oeis.org/A021121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^n mod 117)*10 / 117)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the n-th decimal digit of 1/117 (offset 0). */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)117;                /* denominator of the fraction */
    seq_t r = mod_pow((seq_t)10, n, MOD);        /* r = 10^n mod 117 */
    unsigned long long tmp = (unsigned long long)r * 10ULL;  /* temporary product fits in 64 bits */
    return (seq_t)(tmp / 117ULL);
}
