/*
 * A011897  a(n) = floor(n*(n-1)*(n-2)/15).
 * https://oeis.org/A011897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/15)
 * Range:     u8 n <= 16, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    /* use a wider intermediate to avoid overflow for small seq_t */
    unsigned long long prod = (unsigned long long)n * (n-1) * (n-2);
    return (seq_t)(prod / 15);
}
