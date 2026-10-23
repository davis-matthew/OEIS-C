/*
 * A145067  Zero followed by partial sums of A008865.
 * https://oeis.org/A145067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1}^{n-1} (k^2-2)
 * Range:     i8 n <= 8, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(1) = 0                          */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t kk = (seq_t)k;
        total += kk * kk - 2;
    }
    return total;
}
