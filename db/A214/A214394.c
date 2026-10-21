/*
 * A214394  If n mod 6 = 0 then n/6 else n.
 * https://oeis.org/A214394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, except when n mod 6 = 0 then a(n) = n/6
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an idx_t (uint64_t) – cast to seq_t only when returning */
    if (n % 6 == 0)
        return ((seq_t)n) / 6;
    else
        return (seq_t)n;
}
