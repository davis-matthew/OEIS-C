/*
 * A011874  a(n) = floor(n*(n-1)/21).
 * https://oeis.org/A011874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/21)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    idx_t prod = ((idx_t)n) * ((idx_t)n - 1);
    return (seq_t)(prod / 21);
}
