/*
 * A188012  Positions of 0 in A188011; complement of A188013.
 * https://oeis.org/A188012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*floor((n-1)*phi)+2*(n-1)+3 where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n == 0, offset is 1 */
    idx_t k = n - 1;                 /* k ≥ 0 */
    seq_t k_seq = (seq_t)k;

    /* floor(k * sqrt(5)) = floor_sqrt(5 * k^2) */
    seq_t term = (seq_t)5 * k_seq * k_seq;
    seq_t sqrt_term = floor_sqrt(term);

    /* floor(k * phi) = floor((k + k*sqrt(5)) / 2) */
    seq_t floor_k_phi = (k_seq + sqrt_term) / 2;

    /* a(n) = 3 * floor_k_phi + 2 * k + 3 */
    seq_t result = (seq_t)3 * floor_k_phi + (seq_t)2 * k_seq + (seq_t)3;
    return result;
}
