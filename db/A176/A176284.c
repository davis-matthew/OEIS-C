/*
 * A176284  Triangle T(n,k) = 1 + 3*n*k*(n-k) read by rows.
 * https://oeis.org/A176284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 + 3 * r * k * (r - k) * where * r = floor((sqrt(8*n +
 *            1) - 1)/2 ) (row index) * k = n - r*(r+1)/2 (column index in
 *            that row)
 * Range:     u8 n <= 7, u16 n <= 1052, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n + 1) - 1)/2) */
    seq_t m   = (seq_t)8 * (seq_t)n + (seq_t)1;   /* 8*n + 1 */
    seq_t s   = floor_sqrt(m);                    /* floor sqrt(8*n + 1) */
    idx_t  r  = (idx_t)((s - (seq_t)1) / (seq_t)2);

    /* Compute k = n - r*(r+1)/2 */
    idx_t k = n - r * (r + 1) / 2;

    /* Evaluate T(r,k) = 1 + 3 * r * k * (r - k) */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    seq_t term = rn * kn * (rn - kn);  /* r*k*(r-k) */
    term = term * (seq_t)3;             /* times 3 */
    return term + (seq_t)1;
}
