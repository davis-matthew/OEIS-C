/*
 * A187950  [nr+kr] - [nr] - [kr], where r = (1+sqrt(5))/2, k = 4, [.]=floor.
 * https://oeis.org/A187950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+4)·φ) – floor(n·φ) – 6 where φ = (1+√5)/2. Based
 *            on integer arithmetic: floor(n·φ) = floor( (n + floor( n·√5 )) /
 *            2 ) with floor( n·√5 ) = floor_sqrt(5·n²)
 * Range:     u8 none, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static inline seq_t floor_phi(idx_t m)
{
    /* compute floor(m * φ) */
    seq_t m_seq   = (seq_t)m;
    seq_t m2       = m_seq * m_seq;           /* m²                 */
    seq_t five_m2  = (seq_t)5 * m2;           /* 5·m²               */
    seq_t fsqrt    = floor_sqrt(five_m2);     /* floor(√(5·m²))     */
    return (m_seq + fsqrt) / 2;               /* (m + floor(m√5))/2 */
}

seq_t a(idx_t n)
{
    seq_t phi_n   = floor_phi(n);
    seq_t phi_n4  = floor_phi(n + 4);
    return phi_n4 - phi_n - 6;
}
