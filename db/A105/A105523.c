/*
 * A105523  Expansion of 1-x*c(-x^2) where c(x) is the g.f. of A000108.
 * https://oeis.org/A105523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1; for n>0: if n is even -> a(n)=0; if n is odd -> let
 *            k=(n-1)/2; a(n)=(-1)^(k+1) * Catalan(k) Catalan numbers are
 *            computed by the recurrence C(0)=1, C(m+1)=C(m)*(4*m+2)/(m+2)
 *            (m>=0) This follows from C(n+1)=(2(2n+1)/ (n+2))*C(n)
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 34, i64 n <= 72, i128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & 1) == 0)          /* even n > 0  */
        return (seq_t)0;

    /* odd n: compute Catalan(k) with k=(n-1)/2 */
    idx_t k = (n - 1) / 2;
    seq_t cat = (seq_t)1;      /* C(0) = 1 */

    for (idx_t i = 0; i < k; ++i) {
        /* compute C(i+1) from C(i) */
        cat = cat * ( (seq_t)(4 * i + 2) ) / ( (seq_t)(i + 2) );
    }

    /* sign: (-1)^(k+1) */
    if ( ((k + 1) & 1) == 1 )  /* k+1 odd -> negative sign */
        cat = -cat;

    return cat;
}
