/*
 * A076141  Number of times n occurs as a binary sub-pattern of n^2.
 * https://oeis.org/A076141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of times the binary representation of n occurs as
 *            a substring of the binary representation of n²
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned bitlen_uint64(uint64_t v)
{
    return (v == 0) ? 1 : 64 - __builtin_clzll(v);
}

static inline unsigned bitlen_uint128(unsigned __int128 v)
{
    uint64_t high = (uint64_t)(v >> 64);
    return high ? 64 + bitlen_uint64(high) : bitlen_uint64((uint64_t)v);
}

seq_t a(idx_t n)
{
    /* Compute n² in 128‑bit to avoid overflow for n up to 2^64‑1. */
    unsigned __int128 n2 = (unsigned __int128)n * (unsigned __int128)n;

    unsigned len_n  = (n == 0) ? 1 : bitlen_uint64(n);
    unsigned len_n2 = bitlen_uint128(n2);

    if (len_n > len_n2) return (seq_t)0;

    /* Build a mask with len_n low‑order 1 bits. */
    unsigned __int128 mask;
    if (len_n >= 64) {
        mask = (unsigned __int128)~0ULL;          /* all ones in low 64 bits */
    } else {
        mask = (((unsigned __int128)1 << len_n) - 1);
    }

    unsigned count = 0;
    unsigned max_i = len_n2 - len_n;
    for (unsigned i = 0; i <= max_i; ++i) {
        unsigned shift = len_n2 - len_n - i;          /* shift to align pattern */
        unsigned __int128 part = (n2 >> shift) & mask;
        if (part == (unsigned __int128)n) ++count;
    }
    return (seq_t)count;
}
