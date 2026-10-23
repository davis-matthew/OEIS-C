/*
 * A037223  Number of solutions to non-attacking rooks problem on n X n board
 *          that are invariant under 180-degree rotation.
 * https://oeis.org/A037223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/2))! * 2^(floor(n/2))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented as product_{i=1}^{floor(n/2)} (2*i) */
seq_t a(idx_t n)
{
    idx_t k = n / 2;          /* floor(n/2) for unsigned types */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * ((seq_t)i * 2);
    }
    return result;
}
