/*
 * A169930  Duplicate of A048379.
 * https://oeis.org/A169930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n % 10 + 1) % 10 + (n >= 10 ? ((n / 10) + 1) * 10 : 0)
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 10;
    idx_t base = 0;
    if (n >= 10) {
        base = ((n / 10) + 1) * 10;
    }
    return (seq_t)((r + 1) % 10 + base);
}
