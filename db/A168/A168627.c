/*
 * A168627  a(n) = n^6*(n^5 + 1)/2.
 * https://oeis.org/A168627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 * (n^5 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;    /* Convert index to seq_t    */
    seq_t n2  = x * x;       /* n^2 */
    seq_t n3  = n2 * x;      /* n^3 */
    seq_t n5  = n2 * n3;     /* n^5 = n^2 * n^3           */
    seq_t n6  = n5 * x;      /* n^6 = n^5 * n             */
    seq_t result;

    /* One of the factors is even.  Divide the even factor
     * by 2 first to keep intermediate products within the
     * range of seq_t for all n where the final value fits.  */
    if ((x & 1) == 0) {                 /* n even → n^6 is even   */
        seq_t n6_half = n6 >> 1;        /* n^6 / 2 */
        result = n6_half * (n5 + 1);    /* (n^6/2) * (n^5+1)      */
    } else {                            /* n odd  → n^5+1 is even*/
        seq_t n5_half = (n5 + 1) >> 1;  /* (n^5+1)/2 */
        result = n6 * n5_half;          /* n^6 * ((n^5+1)/2)     */
    }
    return result;
}
