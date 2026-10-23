/*
 * A028991  Odd 9-gonal (or enneagonal) numbers.
 * https://oeis.org/A028991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (28*n^2 + 4*n + 1 + (14*n+1)*(-1)^n)/2
 * Range:     u8 n <= 3, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 28*n^2 once */
    seq_t nn = (seq_t)n * (seq_t)n;       /* n^2                */
    seq_t num;
    if ((n & 1) == 0) {                   /* n even */
        num = 28 * nn + 18 * (seq_t)n + 2;
    } else {                              /* n odd  */
        num = 28 * nn - 10 * (seq_t)n;
    }
    return num / 2;
}
