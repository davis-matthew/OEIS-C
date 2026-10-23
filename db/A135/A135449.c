/*
 * A135449  Period 5: repeat [1, 9, -7, 3, 5].
 * https://oeis.org/A135449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135449: a(n) = 9*a(n-1) mod 11 with a(0)=1.  Values repeat [1,9,-7,3,5]. */
seq_t a(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        r = (r * 9) % 11;
    }
    if (r == 4) return (seq_t)-7;
    return r;
}
