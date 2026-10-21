/*
 * A050403  Partial sums of A051877.
 * https://oeis.org/A050403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+5, 5)*(7*n+6)/6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute binomial(n+5,5) using the multiplicative formula */
    seq_t bin = 1;
    seq_t nn  = (seq_t)n;
    /* C(n+5, 5) = \product_{i=1}^{5} (n+5+1-i)/i */
    bin = bin * (nn + 5) / 1;
    bin = bin * (nn + 4) / 2;
    bin = bin * (nn + 3) / 3;
    bin = bin * (nn + 2) / 4;
    bin = bin * (nn + 1) / 5;

    /* factor = 7*n + 6 */
    seq_t factor = (seq_t)7 * nn + 6;

    /* Avoid overflow when computing (bin * factor)/6 by cancelling 2 and 3 first */
    seq_t x = bin;
    seq_t y = factor;

    if ((y % 2) == 0) {
        y /= 2;          /* y divisible by 2 */
    } else {
        x /= 2;          /* otherwise cancel 2 from x */
    }

    if ((y % 3) == 0) {
        y /= 3;          /* y divisible by 3 */
    } else {
        x /= 3;          /* otherwise cancel 3 from x */
    }

    return x * y;       /* (bin * factor)/6, now safe to multiply */
}
