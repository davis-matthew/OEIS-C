/*
 * A057046  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057046(n)=i(2^n).
 * https://oeis.org/A057046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(2^n)+1, where i(k)=k - T_{d-1} -1, * T_m = m(m+1)/2, d
 *            = ceil( (-1+sqrt(1+8k))/2 ), and 2^n is k
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   floor_sqrt, seq_pow, triangular_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"
#include "../utils/triangular_half.h"

seq_t a(idx_t n)
{
    /* k = 2^n */
    seq_t k = seq_pow((seq_t)2, n);

    /* Compute d = ceil( (-1 + sqrt(1+8k)) / 2 ) */
    seq_t eight_k = k * (seq_t)8;          /* 8*k */
    seq_t sqrt_arg = eight_k + (seq_t)1;   /* 1+8k */
    seq_t t = floor_sqrt(sqrt_arg);        /* floor(sqrt(1+8k)) */
    seq_t d = (t - (seq_t)1) / (seq_t)2;   /* floor((-1+t)/2) */

    /* If T_d < k, increment d */
    if (triangular_half(d) < k)
        d = d + (seq_t)1;

    /* i = k - T_{d-1} - 1  (zero‑based i-coordinate) */
    seq_t prev_tri = triangular_half(d - (seq_t)1);
    seq_t i = k - prev_tri - (seq_t)1;

    /* a(n) = i + 1 (sequence values are 1‑based) */
    return i + (seq_t)1;
}
