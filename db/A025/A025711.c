/*
 * A025711  Index of 5^n within the sequence of the numbers of the form
 *          5^i*10^j.
 * https://oeis.org/A025711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 431, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025711.
   Formula: a(n) = 1 + n*(n+1)/2 - Σ_{k=0..n} ⌊k·log10(2)⌋
   (since ceil(k·log10(5)) = k − floor(k·log10(2))). */
#include <stdint.h>

/* Use a 10^9 scaling for log10(2) to obtain integer truncation: */
static const uint64_t LOG2_NUM = 301029995ULL;   /* log10(2) ≈ 0.301029995 *10^9 */
static const uint64_t SCALE     = 1000000000ULL; /* 10^9 */

static inline uint64_t floor_log10_2(idx_t k)
{
    /* (k * LOG2_NUM) / SCALE  gives floor(k·log10(2)) */
    __int128 prod = (__int128)k * LOG2_NUM;
    return (uint64_t)(prod / SCALE);
}

seq_t a(idx_t n)
{
    /* Sum floor(k*log10(2)) for k = 0 .. n */
    __int128 sum_floor = 0;
    for (idx_t k = 0; k <= n; ++k)
        sum_floor += floor_log10_2(k);

    /* Triangular number n*(n+1)/2 */
    __int128 tri = (__int128)n * (n + 1) / 2;

    /* Final formula */
    __int128 result = 1 + tri - sum_floor;

    return (seq_t)result;
}
