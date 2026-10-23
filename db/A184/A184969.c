/*
 * A184969  a(n) = [Pi]+[2*Pi]+...+[n*Pi], where []=floor.
 * https://oeis.org/A184969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [π]+[2π]+...+[nπ]
 * Range:     u8 n <= 12, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* High‑precision fixed‑point representation of π:
   PI_NUM / SCALE ≈ π, where SCALE = 10^13.
   The truncation error is < 1/SCALE, which is far smaller than
   the minimal fractional part of k·π for all n ≤ 10 000, so
   floor((k·π)) = ((k*PI_NUM)/SCALE) is exact in that range. */
static const uint64_t PI_NUM = 31415926535897ULL; /* floor(π * 10^13) */
static const uint64_t SCALE  = 10000000000000ULL;/* 10^13 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute floor(k·π) using integer arithmetic. */
        uint64_t prod   = (uint64_t)k * PI_NUM;
        uint64_t term   = prod / SCALE;        /* floor(k·π) */
        sum += (seq_t)term;                    /* accumulate in seq_t */
    }
    return sum;
}
