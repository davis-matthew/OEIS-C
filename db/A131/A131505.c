/*
 * A131505  n, -1, n, 2n+2.
 * https://oeis.org/A131505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4k + r) = { * k if r = 0 or 2, * -1 if r = 1, * 2k+2 if r = 3
 *            * } (offset 0)
 * Range:     i8 n <= 92, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t block = n / 4;
    idx_t pos   = n % 4;
    switch (pos) {
        case 0: return (seq_t)block;
        case 1: return (seq_t)-1;
        case 2: return (seq_t)block;
        case 3: return (seq_t)block * 2 + 2;
    }
    return 0;  /* unreachable, but keep compiler happy */
}
