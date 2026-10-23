/*
 * A046820  Number of 1's in binary expansion of 5n.
 * https://oeis.org/A046820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = popcount(5*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* number of 1's in the binary expansion of 5*n */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 5*n is small enough for uint64_t for the intended test range.
       Use 128‑bit intermediate to be safe for larger n.          */
    unsigned __int128 m = (unsigned __int128)n * 5u;
    unsigned long long cnt = 0ull;

    while (m)
    {
        cnt += (unsigned long long)(m & 1u);
        m >>= 1;
    }

    return (seq_t)cnt;
}
