/*
 * A145004  Values of n at which the number of roots of the function
 *          x+n*cos(x) increases.
 * https://oeis.org/A145004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n * π) for n ≠ 1, with a(0) = 0 and a(1) = 3
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The fixed‑point value 3.1415926535 (denominator 10^10) is a lower bound
   for π, accurate enough for n ≤ 10000 and guarantees that
   floor(n·π) = floor(n·(3.1415926535)).  */
#include <stdint.h>

#define PI_LOWER_NUM 31415926535ULL   /* 3.1415926535×10^10  */
#define PI_LOWER_DEN 10000000000ULL   /* 10^10              */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;     /* special case */

    unsigned __int128 prod = (unsigned __int128)n * PI_LOWER_NUM;
    uint64_t floor_val = (uint64_t)(prod / PI_LOWER_DEN);
    return (seq_t)(floor_val + (seq_t)1);
}
