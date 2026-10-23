/*
 * A154281  1,0,0,1 followed by 0,0,0...
 * https://oeis.org/A154281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n == 1 or n == 4, otherwise 0
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative with a(2^2)=1 and a(p^e)=0 for all other prime powers. */
seq_t a(idx_t n)
{
    return (n == 1 || n == 4) ? (seq_t)1 : (seq_t)0;
}
