/*
 * A086796  a(n) is the number of terms in the expansion of
 *          (x+y+z)*(x^2+y^2+z^2)*(x^3+y^3+z^3)*...*(x^n+y^n+z^n).
 * https://oeis.org/A086796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (S+1)(S+2)/2 - m(n) where S = n(n+1)/2 and m(n) is 0 for
 *            n≤1, 1 for n=2, 4 for n=3, and 6 for n≥4
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute S = n(n+1)/2 using seq_t arithmetic */
    seq_t nn = (seq_t)n;
    seq_t S = nn * (nn + 1) / 2;
    /* compute total = (S+1)(S+2)/2 */
    seq_t total = (S + 1) * (S + 2) / 2;
    /* subtract the small correction depending on n */
    seq_t m = 0;
    if (n <= 1)
        m = 0;
    else if (n == 2)
        m = 1;
    else if (n == 3)
        m = 4;
    else
        m = 6;
    return total - m;
}
