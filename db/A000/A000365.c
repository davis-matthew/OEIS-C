/*
 * A000365  Number of genus 0 rooted planar maps with 4 faces and n vertices.
 * https://oeis.org/A000365
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-3)*(n-1)*n*(8n+11)/3 -
 *            binomial(2n-4,n-2)*(2n-3)*(2n-1)*(4n+1)/6 (closed form for the
 *            coefficients of the OEIS g.f.
 *            x^2*(1-sqrt(1-4x))*(7+4x-2*sqrt(1-4x))/(2*(4x-1)^4))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 2;   /* m >= 1 because n >= 3 */
    idx_t k;
    seq_t c = 1;       /* binomial(2k,k) */
    seq_t p = 1;       /* 4^(m-1) */
    seq_t s, t1, t2, d;
    idx_t f1, f2, f3;

    for (k = 1; k <= m; k++)
        c = c * (seq_t)(4 * k - 2) / (seq_t)k;
    for (k = 1; k < m; k++)
        p *= 4;

    /* (m+1)(m+2)(8m+27) is always divisible by 3 */
    s = (seq_t)(m + 1) * (seq_t)(m + 2) * (seq_t)(8 * m + 27) / 3;
    t1 = p * s;

    /* binomial(2m,m)/2 * (2m+1)(2m+3)(4m+9)/3 */
    d = c / 2;
    f1 = 2 * m + 1;
    f2 = 2 * m + 3;
    f3 = 4 * m + 9;
    if (f1 % 3 == 0) f1 /= 3;
    else if (f2 % 3 == 0) f2 /= 3;
    else if (f3 % 3 == 0) f3 /= 3;
    else d /= 3;
    t2 = d * (seq_t)f1 * (seq_t)f2 * (seq_t)f3;

    return t1 - t2;
}
