/*
 * A028362  Total number of self-dual binary codes of length 2n. Totally
 *          isotropic spaces of index n in symplectic geometry of dimension
 *          2n.
 * https://oeis.org/A028362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product_{i=1..n-1} (2^i + 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t pow2   = 1;          // will hold 2^i
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= 2;            // pow2 now equals 2^i
        result *= (pow2 + 1); // multiply by (2^i + 1)
    }
    return result;
}
