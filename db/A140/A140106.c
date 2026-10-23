/*
 * A140106  Number of noncongruent diagonals in a regular n-gon.
 * https://oeis.org/A140106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-2)/2) for n>1, otherwise 0
 * Range:     u8 n <= 512, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)0;
    }
    return ((seq_t)(n - 2)) / 2;
}
