/*
 * A135301  a(1)=1, a(n)=a(n-1)+n^0 if n odd, a(n)=a(n-1)+ n^2 if n is even.
 * https://oeis.org/A135301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = a(n-1) + (n^0 if n is odd, n^2 if n is even) * The
 *            closed‑form (OEIS) is: * a(2m-1) = (4*m^3 - 6*m^2 + 5*m)/3 *
 *            a(2m) = (4*m^3 + 6*m^2 + 5*m)/3 * * where m = floor((n+1)/2) for
 *            odd n, m = n/2 for even n. * This implementation uses the closed
 *            form to obtain O(1) time
 * Range:     u8 n <= 7, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* offset starts at 1; value for 0 is undefined */
    }

    /* Treat odd and even indices separately. */
    idx_t m;
    seq_t num;

    if (n & 1U)        /* n is odd:  n = 2*m - 1  =>  m = (n+1)/2 */
    {
        m = (n + 1) >> 1;                 /* (n+1)/2 */
        num = (seq_t)4 * m * m * m         /* 4*m^3   */
            - (seq_t)6 * m * m             /* -6*m^2  */
            + (seq_t)5 * m;                /* +5*m    */
    }
    else                /* n is even: n = 2*m  =>  m = n/2   */
    {
        m = n >> 1;                        /* n/2 */
        num = (seq_t)4 * m * m * m         /* 4*m^3   */
            + (seq_t)6 * m * m             /* +6*m^2  */
            + (seq_t)5 * m;                /* +5*m    */
    }

    /* The numerator is always divisible by 3 for integer m. */
    return num / 3;
}
