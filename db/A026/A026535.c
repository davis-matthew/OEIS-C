/*
 * A026535  a(n) = t(1+5n) where t = A001285 (Thue-Morse sequence).
 * https://oeis.org/A026535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + t(1+5*n), where t(m) = 0 if popcount(m) even, 1
 *            otherwise
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute parity of the number of 1-bits in a 64‑bit integer */
static inline int parity64(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
}

seq_t a(idx_t n)
{
    /* m = 1 + 5*n (uint64_t wrap‑around is fine for idx_t) */
    uint64_t m = 1ULL + 5ULL * (uint64_t)n;
    int p = parity64(m);       /* 0 or 1 */
    return (seq_t)(1 + p);     /* 1 or 2 */
}
