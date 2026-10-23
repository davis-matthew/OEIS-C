/*
 * A188554  Number of 3 X n binary arrays without the pattern 0 1 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n) = (n^3 + 11*n)/6 + 2 for n>=1
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 1854, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t np = nn * nn * nn;           /* n^3 */
    seq_t prod = (seq_t)11 * nn;        /* 11*n */
    seq_t sum = np + prod;             /* n^3 + 11*n */
    seq_t div = sum / (seq_t)6;         /* divide by 6 */
    seq_t result = div + (seq_t)2;     /* add 2 */
    return result;
}
