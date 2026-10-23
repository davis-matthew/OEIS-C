/*
 * A130862  a(n) = (n-1)*(n+2)*(2*n+11)/2.
 * https://oeis.org/A130862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+2)*(2*n+11)/2
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    seq_t tn = (seq_t)n;
    seq_t part1 = tn - 1;         /* (n-1) */
    seq_t part2 = tn + 2;         /* (n+2) */
    seq_t part3 = 2 * tn + 11;    /* (2*n+11) */
    return part1 * part2 * part3 / 2;
}
