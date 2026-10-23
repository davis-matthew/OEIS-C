/*
 * A165678  Sixth right hand column of triangle A165674.
 * https://oeis.org/A165678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n^5 + 75*n^4 + 340*n^3 + 675*n^2 + 548*n + 120
 * Range:     u8 none, u16 n <= 4, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1764, a(2) = 8028, ...  */
    if (n == 0) {
        return (seq_t)0;          /* offset is 1, but harmless for n==0 */
    }
    seq_t x = (seq_t)n;           /* cast index to seq_t first */
    seq_t res = 6;                /* start with highest coefficient */

    res = res * x + 75;           /* 6*n + 75 => 6*n^2 + 75*n + ... */
    res = res * x + 340;          /* add next lower-degree term */
    res = res * x + 675;
    res = res * x + 548;
    res = res * x + 120;          /* constant term */

    return res;
}
