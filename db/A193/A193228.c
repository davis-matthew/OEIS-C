/*
 * A193228  Truncated octahedron with faces of centered polygons.
 * https://oeis.org/A193228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^3 - 18*n^2 + 8*n - 1
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 710, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert the index to the sequence type first
    seq_t nn = (seq_t)n;

    // Compute powers step by step to avoid any potential overflow in
    // intermediate products for small seq_t types.
    seq_t n2 = nn * nn;   // n^2
    seq_t n3 = n2 * nn;   // n^3

    // Apply the polynomial formula using seq_t arithmetic
    seq_t result = (seq_t)12 * n3 - (seq_t)18 * n2 + (seq_t)8 * nn - (seq_t)1;
    return result;
}
