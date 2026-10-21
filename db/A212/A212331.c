/*
 * A212331  a(n) = 5*n*(n+5)/2.
 * https://oeis.org/A212331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(n+5)/2
 * Range:     u8 n <= 7, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val;
    /* divide by 2 first to avoid overflow when possible */
    if ((nn & (seq_t)1) == 0) {          /* n is even */
        val = 5 * (nn / 2) * (nn + 5);
    } else {                             /* n is odd, n+5 is even */
        val = 5 * nn * ((nn + 5) / 2);
    }
    return val;
}
