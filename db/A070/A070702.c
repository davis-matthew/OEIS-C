/*
 * A070702  Duplicate of A070482.
 * https://oeis.org/A070702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^3 mod 20
 * Range:     i8 n <= 5, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 20 first to keep all intermediate
       values small and avoid overflow even when n is large. */
    idx_t m = n % 20;
    seq_t mm = (seq_t)m;      /* 0 <= mm < 20 */
    seq_t r = mm * mm * mm;   /* at most 19^3 = 6859 */
    r %= 20;
    return r;
}
