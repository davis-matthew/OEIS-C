/*
 * A083032  Numbers that are congruent to {0, 4, 7, 10} mod 12.
 * https://oeis.org/A083032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12·⌊(n‑1)/4⌋ + {0,4,7,10}[ (n‑1) mod 4 ] (n ≥ 1)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Residues that are congruent to 0,4,7,10 (mod 12) */
    static const seq_t offset[4] = {(seq_t)0, (seq_t)4, (seq_t)7, (seq_t)10};

    /* n ≥ 1 by contract.  Use idx_t arithmetic for block and pos. */
    idx_t block = (n - 1U) / 4U;     /* number of complete 12‑blocks */
    idx_t pos   = (n - 1U) % 4U;     /* position inside the block (0‑based) */

    /* All operations here are performed in seq_t to avoid overflow. */
    return ((seq_t)block) * (seq_t)12 + offset[pos];
}
