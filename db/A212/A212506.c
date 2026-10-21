/*
 * A212506  Number of (w,x,y,z) with all terms in {1,...,n} and w<=2x and
 *          y<=2z.
 * https://oeis.org/A212506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((⌊n/2⌋ + n mod 2) * (3*⌊n/2⌋ + 1))^2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                /* floor(n/2) */
    seq_t first  = (seq_t)(k + (n & 1));
    seq_t second = 3 * (seq_t)k + 1;
    seq_t prod   = first * second;
    return prod * prod;
}
