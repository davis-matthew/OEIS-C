/*
 * A014042  Inverse of 33rd cyclotomic polynomial.
 * https://oeis.org/A014042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (18*m^10 - 950*m^9 + 21645*m^8 - 278400*m^7 + 2216844*m^6
 *            * - 11256630*m^5 + 36087705*m^4 - 69333700*m^3 + 70537788*m^2 *
 *            - 27994320*m + 1814400) * * (3*w^2 - 7*w + 2) / 3628800 * where
 *            m = n mod 11, w = (n / 11) mod 3. Sequence is periodic with
 *            period 33
 * Range:     i8 none, i16 none, i32 n <= 1000, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = n mod 11, w = floor(n/11) mod 3 */
    idx_t m_idx = n % 11;
    idx_t w_idx = (n / 11) % 3;

    /* Promote to seq_t for arithmetic */
    seq_t m = (seq_t)m_idx;
    seq_t w = (seq_t)w_idx;

    /* Compute powers of m up to m^10 */
    seq_t m1 = m;                               // m^1
    seq_t m2 = m1 * m;                          // m^2
    seq_t m3 = m2 * m;                          // m^3
    seq_t m4 = m3 * m;                          // m^4
    seq_t m5 = m4 * m;                          // m^5
    seq_t m6 = m5 * m;                          // m^6
    seq_t m7 = m6 * m;                          // m^7
    seq_t m8 = m7 * m;                          // m^8
    seq_t m9 = m8 * m;                          // m^9
    seq_t m10 = m9 * m;                         // m^10

    /* Polynomial in m */
    seq_t poly = 18 * m10
               - 950 * m9
               + 21645 * m8
               - 278400 * m7
               + 2216844 * m6
               - 11256630 * m5
               + 36087705 * m4
               - 69333700 * m3
               + 70537788 * m2
               - 27994320 * m
               + 1814400;

    /* Coefficient depending on w */
    seq_t coef = (3 * w * w) - (7 * w) + 2;

    /* Final result */
    return (poly * coef) / 3628800;
}
