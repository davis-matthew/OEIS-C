/*
 * A147832  Numbers congruent (0,2) mod 14.
 * https://oeis.org/A147832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*(n-1) when n is odd; a(n) = 7*n - 12 when n is even
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* idx_t is unsigned, seq_t may be unsigned or arbitrary‑precision */
    if (n & 1) {
        /* n is odd: a(n) = 7*(n-1) */
        return (seq_t)7 * ((seq_t)n - (seq_t)1);
    } else {
        /* n is even: a(n) = 7*n - 12 */
        return (seq_t)7 * (seq_t)n - (seq_t)12;
    }
}
