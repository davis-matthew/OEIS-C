/*
 * A216173  Number of all possible tetrahedra of any size and orientation,
 *          formed when intersecting the original regular tetrahedron by
 *          planes parallel to its sides and dividing its edges into n equal
 *          parts.
 * https://oeis.org/A216173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/18)*(1 - (-1)^(n mod 3) + 3*n + 7*n^2 + 5*n^3 + n^4)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 294, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = n % 3;
    seq_t t1 = (rem == 1) ? (seq_t)2 : (seq_t)0;    /* 1 - (-1)^(n mod 3) */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t num = t1 + 3 * nn + 7 * n2 + 5 * n3 + n4;
    return num / (seq_t)18;
}
