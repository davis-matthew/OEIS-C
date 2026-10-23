/*
 * A160437  Numerator of Hermite(n, 25/32).
 * https://oeis.org/A160437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25*a(n-1) - 512*(n-1)*a(n-2) with a(0)=1, a(1)=25
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;

    seq_t prev = (seq_t)1;    /* a(0) */
    seq_t cur  = (seq_t)25;   /* a(1) */

    for (idx_t k = 1; k < n; ++k) {
        seq_t term = (seq_t)512 * (seq_t)k * prev;
        seq_t next = (seq_t)25 * cur - term;
        prev = cur;
        cur = next;
    }
    return cur;
}
