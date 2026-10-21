/*
 * A053793  n^2+n modulo 7.
 * https://oeis.org/A053793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n) mod 7
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_mod  = (seq_t)n % 7;          /* n mod 7   */
    seq_t n1_mod = (n_mod + 1) % 7;      /* (n+1) mod 7 */
    seq_t prod   = (n_mod * n1_mod) % 7; /* (n^2 + n) mod 7 */
    return prod;
}
