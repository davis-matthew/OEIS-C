/*
 * A011878  a(n) = floor( n(n-1)/25 ).
 * https://oeis.org/A011878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/25)
 * Range:     u8 n <= 80, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n cast to seq_t to avoid overflow on very large n,
       and the integer division truncates towards zero which is
       equivalent to the floor function for non‑negative arguments. */
    return (seq_t)n * ((seq_t)n - 1) / 25;
}
