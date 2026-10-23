/*
 * A010015  a(0) = 1, a(n) = 25*n^2 + 2 for n > 0.
 * https://oeis.org/A010015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0: a(n) = 25*n^2 + 2
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t nn = n;
    idx_t value = 25ULL * nn * nn + 2;
    return (seq_t)value;
}
