/*
 * A020330  Numbers whose base-2 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n + 2*n*2^floor(log_2(n))
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   seq_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow2.h"

/* Compute floor(log2(v)) for a 64‑bit unsigned integer. */
static inline unsigned floor_log2_uint64(idx_t v)
{
    unsigned r = 0;
    while (v >> 1) {
        ++r;
        v >>= 1;
    }
    return r;
}

/* a(n) according to the given closed form. */
seq_t a(idx_t n)
{
    /* n >= 1 by the problem statement */
    unsigned k = floor_log2_uint64(n);      /* floor(log2(n)) */
    seq_t mult = (seq_t)1 + seq_pow2(k + 1);/* 1 + 2^(k+1) */
    return (seq_t)n * mult;
}
