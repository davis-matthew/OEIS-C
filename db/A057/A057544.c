/*
 * A057544  Maximum cycle length (orbit size) in the rotation permutation of
 *          n+2 side polygon triangularizations.
 * https://oeis.org/A057544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+2 - [n=0] - 2*[n=1] - 2*[n=2]
 * Range:     u8 n <= 253, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;
    return (seq_t)(n + 2);
}
