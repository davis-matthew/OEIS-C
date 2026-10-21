/*
 * A229204  For k>0, a(3k+1) = k*(k-3), a(3k+2) = k*(k-1), a(3k+3) =
 *          k*(k-1)-1.
 * https://oeis.org/A229204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3k+1) = k*(k-3), a(3k+2) = k*(k-1), a(3k+3) = k*(k-1)-1,
 *            offset 1
 * Range:     i8 n <= 37, i16 n <= 547, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t r = m % 3;
    if (r == 1) {
        idx_t k = (m - 1) / 3;
        seq_t kk = (seq_t)k;
        return kk * (kk - 3);
    }
    if (r == 2) {
        idx_t k = (m - 2) / 3;
        seq_t kk = (seq_t)k;
        return kk * (kk - 1);
    }
    /* r == 0 : n = 3k+3 */
    if (m == 3) {
        return (seq_t)0;
    }
    idx_t k = (m - 3) / 3;
    seq_t kk = (seq_t)k;
    return kk * (kk - 1) - 1;
}
