/*
 * A102214  Expansion of (1 + 4*x + 4*x^2)/((1+x)*(1-x)^3).
 * https://oeis.org/A102214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n+2)/2) * ceil((3*n+2)/2)
 * Range:     u8 n <= 9, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the parity of n to choose the simpler quadratic form */
    if ((n & 1) == 0) {                       /* n even: a(2k) = (3k+1)^2   */
        idx_t k   = n >> 1;                   /* k = n/2                   */
        seq_t v   = (seq_t)3 * k + 1;         /* v = 3k + 1                */
        return v * v;                         /* square                           */
    } else {                                  /* n odd : a(2k+1) = (3k+2)(3k+3) */
        idx_t k   = (n - 1) >> 1;             /* k = (n-1)/2                */
        seq_t v1  = (seq_t)3 * k + 2;         /* 3k + 2                     */
        seq_t v2  = (seq_t)3 * k + 3;         /* 3k + 3                     */
        return v1 * v2;                       /* product                        */
    }
}
