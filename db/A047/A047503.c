/*
 * A047503  Numbers that are congruent to {0, 2, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A047503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/6) + [0,2,3,4,5,7][(n-1) mod 6]
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Substitution: numbers NOT congruent to 1 or 6 (mod 8). */

seq_t a(idx_t n)
{
    /* n ≥ 1 by requirement */
    idx_t k = n - 1;          /* zero‑based index */
    idx_t q = k / 6;          /* number of full cycles of 6 valid residues */
    idx_t r = k % 6;          /* position within the current cycle           */

    /* Residue mapping for the 6 admissible residues modulo 8 */
    static const seq_t offs[6] = { 0, 2, 3, 4, 5, 7 };

    /* Compute the value in seq_t (uses only integer arithmetic) */
    seq_t result = 8 * (seq_t)q + offs[r];
    return result;
}
