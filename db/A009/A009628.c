/*
 * A009628  Expansion of e.g.f.: sinh(x)/(1+x).
 * https://oeis.org/A009628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1)*sum_{k=0}^{⌊(n-1)/2⌋} n!/(2k+1)!
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (equivalently, a(n) = (-1)^(n+1)*b(n) with b(0)=0, b(1)=1, b(2)=2 and  
 *            b(n)=n*b(n-1)+b(n-2)-(n-2)*b(n-3) for n>=3) */

seq_t a(idx_t n)
{
    /* handle the offset n==0 */
    if (n == 0) return 0;

    /* base values for the unsigned magnitude sequence b(n) */
    if (n == 1) return 1;          /* a(1) = +1 */
    if (n == 2) return -2;         /* a(2) = -2 */

    /* iterate the recurrence for the magnitude b(n) */
    seq_t b_nm3 = 0;               /* b(n-3), initially b(0) */
    seq_t b_nm2 = 1;               /* b(n-2), initially b(1) */
    seq_t b_nm1 = 2;               /* b(n-1), initially b(2) */
    seq_t b_n   = 0;               /* placeholder for b(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* b(i) = i*b(i-1) + b(i-2) - (i-2)*b(i-3) */
        b_n = (seq_t)i * b_nm1 + b_nm2 - (seq_t)(i - 2) * b_nm3;
        b_nm3 = b_nm2;
        b_nm2 = b_nm1;
        b_nm1 = b_n;
    }

    /* apply the sign (-1)^(n+1): + for odd n, - for even n (n>=1) */
    seq_t sign = (n & 1) ? (seq_t)1 : (seq_t)-1;
    return sign * b_n;
}
