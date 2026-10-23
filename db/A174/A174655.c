/*
 * A174655  Partial sums of A049486.
 * https://oeis.org/A174655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n^3 - 12*n^2 + 34*n - 27 + 3*(-1)^n)/12 for n>1, a(1)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)8 * nn * nn * nn;          /* 8*n^3          */
    seq_t term2 = (seq_t)12 * nn * nn;              /* 12*n^2         */
    seq_t term3 = (seq_t)34 * nn;                   /* 34*n           */
    seq_t m = term1 - term2 + term3 - (seq_t)27;    /* 8n^3 -12n^2+34n-27 */
    if ((n & 1) == 0)   /* even n → +3 */
        m += (seq_t)3;
    else                /* odd n  → -3 */
        m -= (seq_t)3;
    return m / (seq_t)12;
}
