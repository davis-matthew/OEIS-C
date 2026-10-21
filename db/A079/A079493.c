/*
 * A079493  Duplicate of A080020.
 * https://oeis.org/A079493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 367 For n > 1: a(n) = 367 + 6 * Σ_{k=1}^{n-1} c(k) where
 *            c(k) = k if k is odd c(k) = k/2 if k is even Closed form: let m
 *            = n-1 if m is even (m = 2q): a(n) = 367 + 9*q^2 + 3*q if m is
 *            odd (m = 2q+1): a(n) = 367 + 9*q^2 + 15*q + 6
 * Range:     i8 none, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    seq_t q;
    seq_t sq;

    if (m & 1U) {                    /* m is odd */
        q  = (seq_t)((m - 1U) >> 1);   /* q = (n-2)/2 */
        sq = q * q;
        return (seq_t)367 + (seq_t)9 * sq + (seq_t)15 * q + (seq_t)6;
    } else {                          /* m is even */
        q  = (seq_t)(m >> 1);          /* q = (n-1)/2 */
        sq = q * q;
        return (seq_t)367 + (seq_t)9 * sq + (seq_t)3 * q;
    }
}
