/*
 * A030130  Binary expansion contains a single 0.
 * https://oeis.org/A030130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = numbers whose binary expansion contains exactly one zero
 *            digit. The sequence starts with a(1)=0; for n>1 it enumerates
 *            all positive integers with exactly one zero in binary, sorted in
 *            increasing order. Implementation generates the n‑th term by
 *            determining the binary length L that contains...
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 497, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2_u128.h"

/* Integer square root: floor(sqrt(n)) for n <= 2^64-1. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0, bit;
    bit = (idx_t)1 << 62;                 /* highest power of four <= n */
    while (bit > 0) {
        idx_t y = x | bit;
        if ((unsigned __int128)y * y <= n)
            x = y;
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* n-1 because a(1)=0 is a special case; the rest are positive numbers. */
    idx_t m = n - 1;

    /* Find smallest L >= 2 such that L*(L-1)/2 >= m. */
    idx_t L = (1 + isqrt_u64(1 + 8 * m)) / 2;
    while ((L * (L - 1)) / 2 < m)
        ++L;

    /* Count before current length L. */
    idx_t prev = ((L - 1) * (L - 2)) / 2;
    idx_t offset = m - prev;           /* 1-based position within length L */

    /* j is the index of the zero from LSB (0) upwards. */
    idx_t j = L - 1 - offset;

    /* Compute value = (2^L - 1) - 2^j.  Use __int128 to avoid overflow. */
    unsigned __int128 all_ones, zerobit, val128;
    if (L <= 127)
        all_ones = ((unsigned __int128)1 << L) - 1;
    else
        all_ones = pow2_u128(L) - 1;   /* safe for small L; UB avoided */

    if (j <= 127)
        zerobit = ((unsigned __int128)1 << j);
    else
        zerobit = pow2_u128(j);

    val128 = all_ones - zerobit;

    return (seq_t)val128;
}
