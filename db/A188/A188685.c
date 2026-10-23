/*
 * A188685  Partial alternating sums of binomial(3n,n)^2/(2n+1)^2.
 * https://oeis.org/A188685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A188685: Partial alternating sums of (binom(3n,n)/(2n+1))^2.
    a(0) = 1
    a(n) = (C_n)^2 - a(n-1)     for n ≥ 1
    where C_n = binom(3n,n)/(2n+1)
    C_0 = 1
    C_n = C_(n-1) * (3*(3n-1)*(3n-2)) / (2*n*(2n+1))   (exact division)
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t c_prev = (seq_t)1;   /* C(0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t k_seq = (seq_t)k;
        seq_t t1 = (seq_t)3 * k_seq - (seq_t)1;
        seq_t t2 = (seq_t)3 * k_seq - (seq_t)2;
        seq_t num = (seq_t)3 * t1 * t2;                     // 3*(3k-1)*(3k-2)
        seq_t den = (seq_t)2 * k_seq * ((seq_t)2 * k_seq + (seq_t)1); // 2k*(2k+1)

        seq_t c_k = c_prev * num / den;   /* exact division */

        seq_t sq = c_k * c_k;
        seq_t a_k = sq - a_prev;

        a_prev = a_k;
        c_prev = c_k;
    }

    return a_prev;
}
