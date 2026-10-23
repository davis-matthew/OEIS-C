/*
 * A013945  Least d such that period of continued fraction for sqrt(d)
 *          contains n (n^2+2 if n odd, (n/2)^2+1 if n even).
 * https://oeis.org/A013945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2 for odd n * a(n) = (n/2)^2 + 1 for even n *
 *            (equivalently a(n) = (n&1 ? n*n+2 : (n/2)*(n/2)+1) )
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first entry below; behaviour for n=0 is
       irrelevant for the OEIS offset 1.  */
    if (n == 0)
        return 0;               /* not used for offset 1 */

    if ((n & 1U) == 0) {        /* n even */
        seq_t k = (seq_t)(n / 2);   /* floor division preserves integer */
        return k * k + 1;
    } else {                    /* n odd */
        seq_t k = (seq_t)n;
        return k * k + 2;
    }
}
