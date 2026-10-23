/*
 * A187620  a(n) = n^6 - a(n-1), a(0)=1.
 * https://oeis.org/A187620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n + n*(3-5*n^2+3*n^4+n^5)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;             /* n */
    seq_t t2 = t * t;                /* n^2 */
    seq_t t3 = t2 * t;               /* n^3 */
    seq_t t4 = t2 * t2;              /* n^4 */
    seq_t t5 = t4 * t;               /* n^5 */
    seq_t t6 = t5 * t;               /* n^6 */

    /* num = n^6 + 3*n^5 - 5*n^3 + 3*n */
    seq_t num = t6 + ((seq_t)3 * t5) - ((seq_t)5 * t3) + ((seq_t)3 * t);

    seq_t half = num / 2;            /* (n^6 + 3*n^5 - 5*n^3 + 3*n) / 2 */

    if (n & (idx_t)1)                /* odd n: subtract 1 */
        return half - (seq_t)1;
    else                              /* even n: add 1 */
        return half + (seq_t)1;
}
