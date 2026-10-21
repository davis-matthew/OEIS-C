/*
 * A218514  Number of n-colorings of the icosahedral graph.
 * https://oeis.org/A218514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n^8 - 24*n^7 + 260*n^6 - 1670*n^5 +
 *            6999*n^4 - 19698*n^3 + 36408*n^2 - 40240*n + 20170)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 8, u64 n <= 42, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the icosahedral graph cannot be colored with fewer
       than 4 colors, hence a(n) = 0. */
    if (n < 4) return 0;

    /* Promote n to the target integer type before any arithmetic. */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;
    seq_t n8 = n7 * n1;

    /* Separate the polynomial into a sum of positive terms and a sum
       of negative terms; the result is positive for all n >= 4. */
    seq_t pos = n8                /* + n^8          */
              + n6 * 260          /* + 260*n^6      */
              + n4 * 6999         /* + 6999*n^4     */
              + n2 * 36408        /* + 36408*n^2    */
              + 20170;            /* + 20170        */

    seq_t neg = n7 * 24           /* + 24*n^7        */
              + n5 * 1670        /* + 1670*n^5      */
              + n3 * 19698       /* + 19698*n^3     */
              + n1 * 40240;      /* + 40240*n       */

    seq_t poly = pos - neg;       /* total polynomial */

    /* Compute the falling factorial part n*(n-1)*(n-2)*(n-3). */
    seq_t t1 = n1 * (n1 - 1);
    seq_t t2 = t1 * (n1 - 2);
    seq_t fac = t2 * (n1 - 3);

    /* Final value. */
    return fac * poly;
}
