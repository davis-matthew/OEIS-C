/*
 * A089071  Number of liberties a big eye of size n gives in the game of Go.
 * https://oeis.org/A089071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + n - 2, a(1) = 1, a(2) = 2, for n >= 3
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {               /* offset is 1, so behavior below is irrelevant */
        return 0;
    }
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t val = (seq_t)2;        /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        val += (seq_t)i - (seq_t)2;
    }
    return val;
}
