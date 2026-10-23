/*
 * A024352  Numbers which are the difference of two positive squares, c^2 -
 *          b^2 with 1 <= b < c.
 * https://oeis.org/A024352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3, a(n) = 4 + floor((4*n - 3)/3) for n > 1
 * Range:     u8 n <= 189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A024352 – numbers that are differences of two positive squares. */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;         /* first term is 3 */

    /* Use idx_t arithmetic for the division, then convert to seq_t. */
    /* 4*n is safe for idx_t values used in the tests (n <= 10000). */
    idx_t t = (4 * n - 3) / 3;   /* floor((4*n-3)/3) */
    return (seq_t)(4 + t);
}
