/*
 * A212511  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and
 *          y>=3z.
 * https://oeis.org/A212511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 430, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212511: number of quadruples (w,x,y,z) in {1,…,n} with w<2x and y≥3z */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute m = floor((n+1)/2) */
    idx_t m = (n + 1) >> 1;
    /* total_w = m^2 + (n-m)*n */
    seq_t m_sq   = (seq_t)m * (seq_t)m;
    seq_t tw     = m_sq + (seq_t)(n - m) * (seq_t)n;

    /* compute p = floor(n/3) */
    idx_t p = n / 3;
    /* total_y = p*(n+1) - 3*p*(p+1)/2 */
    seq_t p_n1   = (seq_t)p * ((seq_t)n + 1);          /* p*(n+1)      */
    seq_t p_p1   = (seq_t)p * ((seq_t)p + 1);          /* p*(p+1)      */
    seq_t half   = p_p1 / 2;                           /* p*(p+1)/2     */
    seq_t sub    = (seq_t)3 * half;                    /* 3*p*(p+1)/2   */
    seq_t ty     = p_n1 - sub;                         /* p*(n+1) - ... */

    return tw * ty;
}
