/*
 * A073869  a(n) = Sum_{i=0..n} A002251(i)/(n+1).
 * https://oeis.org/A073869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/phi)
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

const uint64_t SCALE = 1000000000000ULL;          /* 10^12 for fixed-point scaling */
const uint64_t NUM   = 1236067977499ULL;          /* (sqrt(5)-1) * SCALE (rounded down) */

/* Compute floor((n+1)/phi) using fixed‑point arithmetic. */
seq_t a(idx_t n)
{
    /* (n+1) * NUM  < 2^128 for all practical n (e.g. n <= 10^12) */
    __int128 prod = (__int128)(n + 1) * NUM;      /* numerator */
    __int128 denom = (__int128)(2 * SCALE);       /* denominator (2 * SCALE) */
    seq_t result = (seq_t)(prod / denom);         /* floor division */
    return result;
}
