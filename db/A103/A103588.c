/*
 * A103588  1's complement of A103582.
 * https://oeis.org/A103588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ((i>>j)&1) where i and j are derived from n by
 *            enumerating the infinite matrix of bits of binary numbers in
 *            little‑endian order via antidiagonals upward. See description
 *            for details
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for a 128‑bit unsigned integer x. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);   /* sqrt(2^128‑1) < 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        /* unsigned division is safe: mid ≤ 2^64 */
        if (mid == 0 || mid <= x / mid) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;   /* low is ceil(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* 1 + 8*n fits in 128 bits */
    unsigned __int128 eight_n = (unsigned __int128)n * 8;
    unsigned __int128 D = eight_n + 1;

    unsigned __int128 sqrtD = isqrt128(D);
    unsigned __int128 s = (sqrtD - 1) >> 1;          /* floor((sqrt(D)-1)/2) */

    unsigned __int128 tri = (s * (s + 1)) >> 1;       /* s*(s+1)/2 */
    unsigned __int128 offset = (unsigned __int128)n - tri;

    idx_t j = (idx_t)offset;
    idx_t i = (idx_t)(s - offset);

    seq_t bit = 0;
    if (j < 64) {
        bit = (((seq_t)i) >> (unsigned long long)j) & 1;
    }
    return bit;
}
