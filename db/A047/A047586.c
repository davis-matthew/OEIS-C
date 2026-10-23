/*
 * A047586  Numbers that are congruent to {2, 3, 5, 6, 7} mod 8.
 * https://oeis.org/A047586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/5) + {2,3,5,6,7}[(n-1) mod 5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer congruent to 2,3,5,6 or 7 modulo 8. */
seq_t a(idx_t n)
{
    /* n is >= 1 as per OEIS offset. */
    idx_t q = (n - 1) / 5;   /* integer quotient */
    idx_t r = (n - 1) % 5;   /* integer remainder */

    /* Residues of the allowed congruence classes modulo 8. */
    static const seq_t residues[5] = { 2, 3, 5, 6, 7 };

    /* Convert quotient to seq_t to avoid overflow, multiply by 8, then add the residue. */
    return (seq_t)q * 8 + residues[(int)r];
}
