/*
 * A035106  1, together with numbers of the form k*(k+1) or k*(k+2), k > 0.
 * https://oeis.org/A035106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(n+2)/4 for n ≥ 2, a(1) = 1
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset case */
    if (n == 1)
        return (seq_t)1;

    seq_t m = (seq_t)n;
    return m * (m + 2) / (seq_t)4;
}
