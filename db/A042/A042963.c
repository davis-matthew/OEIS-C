/*
 * A042963  Numbers congruent to 1 or 2 mod 4.
 * https://oeis.org/A042963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * floor((n-1)/2) + (n%2 ? 1 : 2) (for n ≥ 1)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = floor((n-1)/2) in idx_t to avoid overflow in seq_t */
    idx_t k = (n - 1) / 2;
    /* Compute 4*k as seq_t and add the offset */
    seq_t result = (seq_t)4 * (seq_t)k;
    result += (n & 1) ? (seq_t)1 : (seq_t)2;
    return result;
}
