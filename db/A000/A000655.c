/*
 * A000655  a(n) = number of letters in a(n-1), a(1) = 1 (in English).
 * https://oeis.org/A000655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(1) = 1, a(2) = 3, a(3) = 5, a(n) = 4 for n > 3
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    return (seq_t)4;
}
