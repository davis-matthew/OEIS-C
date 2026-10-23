/*
 * A131211  a(n)=(n^5-n-30)/30.
 * https://oeis.org/A131211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n^5 - n - 30)/30
 * Range:     i8 n <= 5, i16 n <= 15, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Intermediates are kept in a 128‑bit type to avoid overflow on
       64‑bit seq_t builds.  The final result is cast back to seq_t
       (which may be an arithmetic type or an arbitrary‑precision
       class that can be constructed from an integer).  */
    __int128 v = (__int128)n * n * n * n * n; /* n^5 */
    v -= n;
    v -= 30;
    v /= 30;
    return (seq_t)v;
}
