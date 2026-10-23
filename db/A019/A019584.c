/*
 * A019584  a(n) = n^2*(n-1)^3/4.
 * https://oeis.org/A019584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n-1)^3/4
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 112, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A019584.  The value is
 *  n^2*(n-1)^3 divided by 4.  For n=0 or 1 the result is 0. */
seq_t a(idx_t n)
{
    /* Handle the low indices explicitly */
    if (n < 2)
        return (seq_t)0;

    seq_t nn  = (seq_t)n;         /* n          */
    seq_t nm1 = (seq_t)(n - 1);   /* n-1        */
    seq_t res;

    /* For even n, n^2 contains the factor 4
     * For odd n, (n-1)^3 contains the factor 4  */
    if ((n & 1ULL) == 0)           /* n is even      */
    {
        seq_t n2 = nn * nn;        /* n^2            */
        n2 /= 4;                   /* divide by 4    */
        /* Multiply with (n-1)^3  */
        seq_t t = nm1 * nm1;      /* (n-1)^2        */
        t *= nm1;                 /* (n-1)^3        */
        res = n2 * t;             /* n^2/4 * (n-1)^3*/
    }
    else                          /* n is odd       */
    {
        seq_t t = nm1 * nm1;      /* (n-1)^2        */
        t *= nm1;                 /* (n-1)^3        */
        t /= 4;                   /* divide by 4    */
        seq_t n2 = nn * nn;      /* n^2            */
        res = n2 * t;             /* n^2 * (n-1)^3/4*/
    }

    return res;
}
