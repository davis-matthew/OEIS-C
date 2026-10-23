/*
 * A047439  Numbers that are congruent to {0, 1, 5, 6} mod 8.
 * https://oeis.org/A047439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/4) + {0,1,5,6}[(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 4;
    idx_t r = (n - 1) % 4;
    const seq_t offsets[4] = {(seq_t)0, (seq_t)1, (seq_t)5, (seq_t)6};
    return ((seq_t)k << 3) + offsets[r];
}
