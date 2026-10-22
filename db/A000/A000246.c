/*
 * A000246  Number of permutations in the symmetric group S_n that have odd
 *          order.
 * https://oeis.org/A000246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = (2k-1)*a(2k-1), a(2k+1) = (2k+1)*a(2k), a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        if (i & 1)
            r *= (seq_t)i;
        else
            r *= (seq_t)(i - 1);
    }
    return r;
}
