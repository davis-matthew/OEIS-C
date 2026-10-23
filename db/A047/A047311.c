/*
 * A047311  Numbers that are congruent to {4, 5, 6} mod 7.
 * https://oeis.org/A047311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + 4 + ((n-1)%3)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For offset 1; behaviour for n < 1 is undefined */
    idx_t k = (n - 1) / 3;          /* block number (0‑based) */
    idx_t r = (n - 1) % 3;          /* position inside the block (0,1,2) */
    return ((seq_t)7 * k) + 4 + (seq_t)r;
}
