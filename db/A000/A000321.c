/*
 * A000321  H_n(-1/2), where H_n(x) is Hermite polynomial of degree n.
 * https://oeis.org/A000321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*(n-1)*a(n-2), a(0) = 1, a(1) = -1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 17, i64 n <= 28, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 1;   /* a(i-2) */
    seq_t q = -1;  /* a(i-1) */
    idx_t i;
    if (n == 0) return p;
    if (n == 1) return q;
    for (i = 2; i <= n; i++) {
        seq_t r = -q - (seq_t)(2 * (i - 1)) * p;
        p = q;
        q = r;
    }
    return q;
}
