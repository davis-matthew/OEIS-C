/*
 * A182719  Numbers of the form 5k + 4, 7k + 5, or 11k + 6.
 * https://oeis.org/A182719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer that is congruent to 4 (mod 5)
 *            or to 5 (mod 7) or to 6 (mod 11)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; n=0 is undefined */
    seq_t count = 0;
    for (idx_t i = 1; ; ++i) {
        if (i % 5 == 4 || i % 7 == 5 || i % 11 == 6) {
            ++count;
            if (count == (seq_t)n)
                return (seq_t)i;
        }
    }
}
