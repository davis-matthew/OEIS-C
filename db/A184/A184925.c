/*
 * A184925  n+[rn/s]+[tn/s]+[un/s], where []=floor and r=1, s=sqrt(3),
 *          t=sqrt(5), u=sqrt(7).
 * https://oeis.org/A184925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/√3) + floor(n*√(5/3)) + floor(n*√(7/3))
 * Range:     u8 n <= 58, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The three floor terms are computed as floor(√(k·n²/3)) for k = 1,5,7 using only integer arithmetic. */
#include <stdint.h>

/* Integer square root (floor) using the algorithm from "Hacker's Delight".  */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;            /* largest power of 4 <= 2^64   */
    while (bit > x) bit >>= 2;         /* find highest bit <= x          */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* compute floor(n / √3)   -> floor(√(n² / 3)) */
    idx_t n_sq = n * n;           /* n² fits 64‑bit for the intended range (n ≤ 10000) */
    idx_t a1 = isqrt_u64(n_sq / 3);

    /* compute floor(n * √(5/3)) -> floor(√(5·n² / 3)) */
    idx_t a2 = isqrt_u64((5 * n_sq) / 3);

    /* compute floor(n * √(7/3)) -> floor(√(7·n² / 3)) */
    idx_t a3 = isqrt_u64((7 * n_sq) / 3);

    /* sum the parts */
    return (seq_t)n + (seq_t)a1 + (seq_t)a2 + (seq_t)a3;
}
