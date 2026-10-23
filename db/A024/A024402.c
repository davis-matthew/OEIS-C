/*
 * A024402  [ (4th elementary symmetric function of S(n))/(2nd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 2 mod 3}.
 * https://oeis.org/A024402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4(S(n)) / e2(S(n)) ) where S(n) = { first n+3
 *            positive integers ≡ 2 (mod 3) }. e2(S) and e4(S) are the 2nd and
 *            4th elementary symmetric functions of S. These are computed via
 *            Newton's identities using the power sums p_k = Σ (a_i)^k for k =
 *            1..4. The results are integer; the floor i...
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for standard integer types */

seq_t a(idx_t n)
{
    /* m = size of S(n) = n + 3 */
    const seq_t m = (seq_t)n + 3;

    /* Power sums for the arithmetic progression a_i = 3*i + 2, i = 0..m-1 */

    /* p1 = sum a_i = (3*m*m + m) / 2 */
    const seq_t p1 = ( (3 * m * m) + m ) / 2;

    /* p2 = m * (6*m*m + 3*m - 1) / 2 */
    const seq_t p2 = ( m * ( 6 * m * m + 3 * m - 1 ) ) / 2;

    /* auxiliary variables */
    const seq_t m_minus1 = m - 1;
    const seq_t two_m_minus1 = (2 * m) - 1;

    /* s3 = Σ i^3 = m*m*(m-1)^2 / 4 */
    const seq_t s3 = ( m * m * m_minus1 * m_minus1 ) / 4;

    /* s2 = Σ i^2 = m*(m-1)*(2*m-1) / 6 */
    const seq_t s2 = ( m * m_minus1 * two_m_minus1 ) / 6;

    /* s1 = Σ i   = m*(m-1) / 2 */
    const seq_t s1 = ( m * m_minus1 ) / 2;

    /* s4 = Σ i^4 = m*(m-1)*(2*m-1)*(3*m*m - 3*m -1) / 30 */
    const seq_t s4 = ( m * m_minus1 * two_m_minus1 *
                       ( 3 * m * m - 3 * m - 1 ) ) / 30;

    /* p3 = Σ a_i^3 = 27*s3 + 54*s2 + 36*s1 + 8*m
           (note: 54*s2 means 9*m*(m-1)*(2*m-1)*2? But we use the formula directly) */
    const seq_t p3 = (27 * m * m * m_minus1 * m_minus1) / 4
                    + (9 * m * m_minus1 * two_m_minus1)
                    + (18 * m * m_minus1)
                    + (8 * m);

    /* p4 = Σ a_i^4 = 81*s4 + 216*s3 + 216*s2 + 96*s1 + 16*m */
    const seq_t p4 = (81 * s4) + (216 * s3) + (216 * s2) + (96 * s1) + (16 * m);

    /* Elementary symmetric sums via Newton's identities */
    const seq_t e2 = (p1 * p1 - p2) / 2;
    const seq_t e4 = ( p1 * p1 * p1 * p1
                       - 6 * p1 * p1 * p2
                       + 3 * p2 * p2
                       + 8 * p1 * p3
                       - 6 * p4 ) / 24;

    /* floor(e4 / e2) */
    return e4 / e2;
}
