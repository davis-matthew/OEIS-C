/*
 * A108357  Expansion of (1+x^2+x^4)/(1-x^8).
 * https://oeis.org/A108357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n%8 ∈ {0,2,4}, otherwise 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the expansion of (1+x^2+x^4)/(1-x^8). */
seq_t a(idx_t n)
{
    idx_t r = n & 7;          /* n % 8 (n is unsigned) */
    return (r == 0 || r == 2 || r == 4) ? (seq_t)1 : (seq_t)0;
}
