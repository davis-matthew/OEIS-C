/*
 * A130877  Numbers that are congruent to {0, 5} mod 9.
 * https://oeis.org/A130877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*(n-1) + ((n-1)&1)) / 2 for n >= 1
 * Range:     u8 n <= 57, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* just in case; offset starts at 1 */
        return 0;
    seq_t m = (seq_t)(n - 1);
    return ((seq_t)9 * m + (m & 1)) / 2;
}
