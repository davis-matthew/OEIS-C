/*
 * A047624  Numbers that are congruent to {0, 1, 3, 5} mod 8.
 * https://oeis.org/A047624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,1,3,5][(n-1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t idx   = n - 1;
    idx_t block = idx / 4;
    idx_t pos   = idx % 4;
    static const seq_t offs[4] = { (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)5 };
    return ((seq_t)block * (seq_t)8) + offs[pos];
}
