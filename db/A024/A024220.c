/*
 * A024220  a(n) = [ (3rd elementary symmetric function of S(n))/(first
 *          elementary symmetric function of S(n)) ], where S(n) = {first n+2
 *          positive integers congruent to 1 mod 3}.
 * https://oeis.org/A024220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e3 / e1 ), where e1 = sum_{i=0}^{m-1} (1+3i), e3 =
 *            elementary symmetric function of degree 3 of {1+3i}, m = n + 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = n + 2 (number of terms in the arithmetic progression) */
    seq_t m       = (seq_t)n + (seq_t)2;

    /* power sums for the sequence a_i = 1 + 3k, k = 0 .. m-1 */
    /* sum_k   = Σk = m(m-1)/2                      */
    seq_t sum_k   = (m * (m - (seq_t)1)) / (seq_t)2;

    /* sum_k2  = Σk^2 = (m-1)m(2m-1)/6               */
    seq_t sum_k2  = ((m - (seq_t)1) * m *
                     ((seq_t)2 * m - (seq_t)1)) / (seq_t)6;

    /* sum_k3  = Σk^3 = (Σk)^2                       */
    seq_t sum_k3  = sum_k * sum_k;

    /* p1 = Σa_i  = m + 3 Σk                           */
    seq_t p1 = m + ((seq_t)3 * sum_k);

    /* p2 = Σa_i^2  = m + 6 Σk + 9 Σk^2                */
    seq_t p2 = m + ((seq_t)6 * sum_k) + ((seq_t)9 * sum_k2);

    /* p3 = Σa_i^3  = m + 9 Σk + 27 Σk^2 + 27 Σk^3     */
    seq_t p3 = m + ((seq_t)9 * sum_k) + ((seq_t)27 * sum_k2)
               + ((seq_t)27 * sum_k3);

    /* e3 = (p1^3 - 3 p1 p2 + 2 p3) / 6  (Newton's identity) */
    seq_t p1_cub  = p1 * p1 * p1;
    seq_t term2   = ((seq_t)3 * p1 * p2);
    seq_t term3   = ((seq_t)2 * p3);
    seq_t numer   = p1_cub - term2 + term3;        /* exact division by 6 */

    seq_t e3 = numer / (seq_t)6;

    /* a(n) = floor(e3 / p1) */
    return e3 / p1;
}
