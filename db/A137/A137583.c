/*
 * A137583  Number of elements in the n-th period of the Janet periodic table
 *          of elements.
 * https://oeis.org/A137583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   a(n) = 2 * ((n+1)/2)^2
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n + 1) / 2;   /* ceil(n/2) for integer division */
    return ((seq_t)2) * m * m;
}
