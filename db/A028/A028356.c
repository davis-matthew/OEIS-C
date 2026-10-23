/*
 * A028356  Simple periodic sequence underlying clock sequence A028354.
 * https://oeis.org/A028356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(((n+3) mod 6)-3)+1 (periodic sequence: 1,2,3,4,3,2
 *            repeating every 6 terms)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[6] = {1, 2, 3, 4, 3, 2};
    return table[n % 6];
}
