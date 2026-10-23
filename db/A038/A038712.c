/*
 * A038712  Let k be the exponent of highest power of 2 dividing n (A007814);
 *          a(n) = 2^(k+1)-1.
 * https://oeis.org/A038712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = 2^(v2(n)+1) - 1 where v2(n) is the exponent of the
 *            highest power of 2 dividing n (i.e. the number of trailing zeros
 *            in the binary representation of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1 – unused value */

    idx_t tmp = n;
    idx_t k   = 0;                          /* count trailing zeros */

    while ((tmp & 1) == 0)
    {
        k++;
        tmp >>= 1;
    }

    /* compute 2^(k+1) - 1 using a 128‑bit intermediate to avoid overflow
       for the maximum possible exponent (k <= 63 for 64‑bit n). */
    unsigned __int128 val = (((unsigned __int128)1) << (k + 1)) - 1;

    return (seq_t)val;
}
