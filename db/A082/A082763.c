/*
 * A082763  Roman numeral contains an asymmetric symbol (L).
 * https://oeis.org/A082763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor((n-1)/50)*100 + 40 + ((n-1) mod 50)
 * Range:     u8 n <= 116, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t n1 = n - 1;          /* n >= 1 by definition */
    idx_t q = n1 / 50;         /* number of complete 50‑term blocks */
    idx_t r = n1 % 50;         /* offset within the block */
    return ((seq_t)q) * 100 + 40 + (seq_t)r;
}
