/*
 * A047255  Numbers that are congruent to {1, 2, 3, 5} mod 6.
 * https://oeis.org/A047255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 6*⌊(n-1)/4⌋ + {1,2,3,5}[ (n-1) mod 4 ]
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for offset 1, but safe */
    static const seq_t pat[4] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5 };
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;
    return ((seq_t)6 * (seq_t)q) + pat[r];
}
