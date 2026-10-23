/*
 * A163380  a(n) = the (decimal equivalent of the) largest integer that can be
 *          made by rotating the binary digits of n any number of positions to
 *          the left or right.
 * https://oeis.org/A163380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = the largest integer obtained by cyclically rotating the
 *            binary representation of n (without leading zeros)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t bitlen(idx_t n)
{
    idx_t len = 0;
    while (n) {
        ++len;
        n >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence. */
    idx_t m = bitlen(n);          /* number of significant bits */
    seq_t best = (seq_t)n;        /* initial best is the original number */

    uint64_t mask = (m == 64) ? ~0ULL : ((1ULL << m) - 1);

    /* Iterate over all non‑trivial rotations. */
    for (idx_t k = 1; k < m; ++k) {
        /* Perform rotation on the underlying 64‑bit value. */
        __uint128_t r128 = (((__uint128_t)n) << k) |
                           (((__uint128_t)n) >> (m - k));
        uint64_t r = (uint64_t)(r128 & mask);  /* rotate result back to 64 bits */

        if ((seq_t)r > best)
            best = (seq_t)r;
    }

    return best;
}
