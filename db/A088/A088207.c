/*
 * A088207  a(n) = Sum_{k=0..n} floor(k*phi^2) where phi=(1+sqrt(5))/2.
 * https://oeis.org/A088207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(k*phi^2), with phi=(1+sqrt5)/2
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   sqrt5k2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt5k2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk       = (seq_t)k;
        seq_t sqrtk5   = sqrt5k2(k);
        seq_t floor_k_phi   = (kk + sqrtk5) / 2;   /* floor(k*phi) */
        seq_t floor_k_phi2  = kk + floor_k_phi;    /* floor(k*phi^2) = k + floor_k_phi */
        sum += floor_k_phi2;
    }
    return sum;   /* n==0 returns 0 automatically */
}
