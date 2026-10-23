/*
 * A167614  a(n) = (n^2 + 3*n + 8)/2.
 * https://oeis.org/A167614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 3*n + 8)/2
 * Range:     u8 n <= 14, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1, but guard against 0 in case of accidental call */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)3 * nn + (seq_t)8;
    return val / 2;
}
