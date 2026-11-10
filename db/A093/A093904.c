/*
 * A093904  Duplicate of A093344.
 * https://oeis.org/A093904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A093344(n-1) = (n-1)! * Sum_{i=1..n-1}
 *            (1/i)*Sum_{j=0..i-1} 1/j!. Recurrence: b(0)=0, c(0)=1;
 *            b(k)=k*b(k-1)+c(k-1), c(k)=k*c(k-1)+1; a(n)=b(n-1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t b = 0;
    seq_t c = 1;
    idx_t m = n - 1;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t kk = (seq_t)k;
        b = kk * b + c;
        c = kk * c + 1;
    }
    return b;
}
