/*
 * A213847  Rectangular array: (row n) = b**c, where b(h) = 4*h-1, c(h) =
 *          2*n-3+2*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row,col), where T(n,m) = Σ_{i=1}^{m}
 *            (4*i-1)(2n+2m-2i-1)
 * Range:     u8 n <= 15, u16 n <= 465, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Helper: integer square root for 64-bit unsigned integers */
static idx_t isqrt_u64(idx_t x)
{
    /* use long double sqrt for an initial approximation */
    long double fx = sqrt((long double)x);
    idx_t r = (idx_t)fx;
    /* refine upward */
    while (((__uint128_t)(r + 1) * (r + 1)) <= x) r++;
    /* refine downward */
    while (((__uint128_t)r * r) > x) r--;
    return r;
}

/* Compute the rectangular array entry T(n,m) */
static seq_t T_value(seq_t n_s, seq_t m_s)
{
    /* D = 2n + 2m - 1 */
    seq_t D = (seq_t)2 * n_s + (seq_t)2 * m_s - (seq_t)1;
    /* S1 = m(m+1)/2 */
    seq_t S1 = m_s * (m_s + (seq_t)1) / (seq_t)2;
    /* S2 = m(m+1)(2m+1)/6 */
    seq_t S2 = m_s * (m_s + (seq_t)1) * ((seq_t)2 * m_s + (seq_t)1) / (seq_t)6;
    /* part1 = 2*(4n+4m-1)*S1 */
    seq_t part1 = (seq_t)2 * ((seq_t)4 * n_s + (seq_t)4 * m_s - (seq_t)1) * S1;
    /* part2 = D*m */
    seq_t part2 = D * m_s;
    /* part3 = 8*S2 */
    seq_t part3 = (seq_t)8 * S2;
    return part1 - part2 - part3;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n<1 but return 0 */

    /* find diagonal d such that (d-1)d/2 >= n */
    idx_t d = (idx_t)( (idx_t)1 + isqrt_u64( (idx_t)1 + (idx_t)8 * n ) ) / (idx_t)2;
    /* adjust in case of rounding errors */
    while ( (__uint128_t)d * (d - 1) / 2 < n ) d++;
    while ( (__uint128_t)(d - 1) * (d - 2) / 2 >= n ) d--;

    /* offset within diagonal: zero-based */
    idx_t prev_total = (idx_t)( (d - 2) * (d - 1) / 2 );
    idx_t offset = n - prev_total - 1; /* 0..(d-2) */

    idx_t row = offset + 1;                      /* n coordinate in array */
    idx_t col = d - row;                         /* m coordinate */

    seq_t n_s = (seq_t)row;
    seq_t m_s = (seq_t)col;
    return T_value(n_s, m_s);
}
