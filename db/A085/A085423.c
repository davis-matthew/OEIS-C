/*
 * A085423  a(n) = floor(log_2(3n)).
 * https://oeis.org/A085423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_2(3*n)) (offset 1)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

/* Compute floor(log2(x)) for a 128‑bit unsigned integer x > 0.
   x is expected to be in the range [1, 2^128-1]. */
static inline int floor_log2_uint128(unsigned __int128 x)
{
    int lg = 0;
    /* If the high 64 bits are non‑zero, shift them out and add 64 to the log. */
    if (x >> 64) {
        x >>= 64;
        lg += 64;
    }
    /* Now x fits into a 64‑bit word. Use __builtin_clzll to find the log. */
    uint64_t v = (uint64_t)x;
    /* __builtin_clzll is defined for non‑zero values; x > 0 guarantees that. */
    lg += 63 - __builtin_clzll(v);
    return lg;
}

seq_t a(idx_t n)
{
    /* n >= 1 as per problem statement */
    unsigned __int128 prod = (unsigned __int128)n * 3;
    return (seq_t)floor_log2_uint128(prod);
}
