/*
 * A060079  Duplicate of A052502.
 * https://oeis.org/A060079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ∏_{k=1..n} (3*k-2)(3*k-1), a(0)=1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * (seq_t)(3 * i - 2);
        res = res * (seq_t)(3 * i - 1);
    }
    return res;
}
