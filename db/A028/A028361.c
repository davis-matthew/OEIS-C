/*
 * A028361  Number of totally isotropic spaces of index n in orthogonal
 *          geometry of dimension 2n.
 * https://oeis.org/A028361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product_{i=0..n-1} (2^i + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;   /* accumulated product */
    seq_t pow2   = 1;   /* current value of 2^i */

    for (idx_t i = 0; i < n; ++i) {
        seq_t term = pow2 + 1;   /* 2^i + 1 */
        result *= term;         /* multiply into product */
        pow2 <<= 1;             /* next power of two: 2^(i+1) */
    }
    return result;
}
