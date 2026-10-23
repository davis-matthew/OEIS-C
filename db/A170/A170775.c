/*
 * A170775  a(n) = n^8*(n^3 + 1)/2.
 * https://oeis.org/A170775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8*(n^3 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 3390,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ni = (seq_t)n;            /* convert index to seq_t */
    seq_t n2 = ni * ni;             /* n^2 */
    seq_t n4 = n2 * n2;             /* n^4 */
    seq_t n8 = n4 * n4;             /* n^8 */
    seq_t n3 = n2 * ni;             /* n^3 */
    seq_t n3p1 = n3 + 1U;           /* n^3 + 1 */

    /* Divide by 2 on the factor that is even, then multiply */
    if ((n & 1U) == 0U) {           /* n is even, n^8 is even */
        seq_t half = n8 / 2;
        return half * n3p1;
    } else {                        /* n is odd, n^3+1 is even */
        seq_t half = n3p1 / 2;
        return n8 * half;
    }
}
