/*
 * A047248  Numbers that are congruent to {0, 2, 3, 4, 5} (mod 6).
 * https://oeis.org/A047248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047248: numbers that are congruent to {0,2,3,4,5} (mod 6)
 * Formula: a(n) = 6*floor((n-1)/5) + {0,2,3,4,5}[(n-1) mod 5]   for n ≥ 1 */
seq_t a(idx_t n)
{
    /* zero‑based linear index of the term */
    idx_t m = n - 1;
    /* number of complete blocks of 5 numbers (each block spans 6 integers) */
    seq_t q = (seq_t)(m / 5);
    /* position inside the current block (0 – 4) */
    idx_t ridx = m % 5;
    /* pre‑computed allowed residues modulo 6 */
    static const seq_t offsets[5] = {0, 2, 3, 4, 5};
    return 6 * q + offsets[ridx];
}
