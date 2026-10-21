/*
 * A218327  Even octagonal pyramidal numbers (A002414).
 * https://oeis.org/A218327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-(-1)^n+1)*(4*n-(-1)^n+3)*(4*n-(-1)^n)/8
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * (seq_t)4;
    if ((n & (idx_t)1) == 0) {   /* n even */
        t = t - (seq_t)1;
    } else {                     /* n odd  */
        t = t + (seq_t)1;
    }
    seq_t prod = t * (t + (seq_t)1) * (t + (seq_t)3);
    return prod / (seq_t)8;
}
