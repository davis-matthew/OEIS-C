/*
 * A089890  Number of subsets of {1,.., n} containing exactly two squares.
 * https://oeis.org/A089890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n))*(floor(sqrt(n))-1)/2 * 2^(n -
 *            floor(sqrt(n)))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0, bit = (idx_t)1ULL << 62;

    while (bit > x)
        bit >>= 2;

    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
        {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* k = floor(sqrt(n)) */
    idx_t k = isqrt_u64(n);

    /* first part: k*(k-1)/2 */
    seq_t first = ((seq_t)k * ((seq_t)k - 1)) / 2;

    /* second part: 2^(n-k) */
    seq_t pow2 = (seq_t)1;
    idx_t exp = n - k;
    while (exp--)
        pow2 <<= 1;

    return first * pow2;
}
