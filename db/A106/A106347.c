/*
 * A106347  A generalized Fredholm-Rueppel sequence.
 * https://oeis.org/A106347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n = 0, -1 if n+1 is a power of 2, 2 if n+2 is a
 *            power of 2, 0 otherwise
 * Range:     i8 n <= 124, i16 n <= 124, i32 n <= 124, i64 n <= 124,
 *            i128 n <= 124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* test power of 2: x & (x-1) == 0 */
    if (((n + 1) & n) == 0)
        return (seq_t)-1;
    if (((n + 2) & (n + 1)) == 0)
        return (seq_t)2;
    return (seq_t)0;
}
