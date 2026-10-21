/*
 * A088055  a(n) = n!*n^n - ((n^(n+1)-1)/(n-1) - 1) for n>1 with a(1)=0.
 * https://oeis.org/A088055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * n^n - ( n * (n^n - 1) ) / (n-1) for n > 1 * with
 *            a(1) = 0 (a(0) unused). This is equivalent to * a(n) =
 *            A061711(n) - A031972(n) as given in the OEIS entry. * The
 *            implementation uses only integer arithmetic and works * with any
 *            `seq_t` type that supports ordinary arithmetic operators....
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0 by definition */
        return (seq_t)0;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute n^n */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)n;

    /* num = n! * n^n */
    seq_t num = fact * pow;

    /* G(n) = n * (n^n - 1) / (n - 1) */
    seq_t temp = pow - (seq_t)1;          /* n^n - 1 */
    temp *= (seq_t)n;                     /* n * (n^n - 1) */
    seq_t G = temp / ((seq_t)n - (seq_t)1);

    /* a(n) = num - G */
    return num - G;
}
