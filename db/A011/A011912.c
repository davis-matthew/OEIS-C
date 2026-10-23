/*
 * A011912  a(n) = floor(n*(n-1)*(n-2)/30).
 * https://oeis.org/A011912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/30)
 * Range:     u8 n <= 20, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    unsigned __int128 prod = (unsigned __int128)n * (n - 1) * (n - 2);
    return (seq_t)(prod / 30);
}
