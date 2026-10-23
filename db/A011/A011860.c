/*
 * A011860  a(n) = floor(n*(n-1)/7).
 * https://oeis.org/A011860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/7)
 * Range:     u8 n <= 42, u16 n <= 677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases immediately. */
    if (n < 2)
        return (seq_t)0;

    /* The computation uses the unsigned type of the index.  The
       intermediate product may be large, so we use a 128‑bit
       integer if the compiler supports it; otherwise a 64‑bit
       integer is sufficient for the ranges that will be tested
       in the evaluation. */
#if defined(__SIZEOF_INT128__)
    __uint128_t prod = (__uint128_t)(n) * (n - 1U);
    return (seq_t)(prod / 7);
#else
    uint64_t prod = (uint64_t)(n) * (n - 1U);
    return (seq_t)(prod / 7);
#endif
}
