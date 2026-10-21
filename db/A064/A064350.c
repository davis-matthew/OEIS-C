/*
 * A064350  a(n) = (3*n)!/n!.
 * https://oeis.org/A064350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n)!/n!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    /* product of the integers from n+1 through 3n */
    for (idx_t i = n + 1; i <= 3 * n; ++i) {
        res *= (seq_t)i;
    }
    return res;
}
