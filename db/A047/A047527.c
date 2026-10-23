/*
 * A047527  Numbers that are congruent to {0, 1, 2, 7} mod 8.
 * https://oeis.org/A047527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {0,1,2,7}[ (n-1) mod 4 ]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent to the OEIS formula: a(n) = 3*n-4*floor((n-2)/4)-6+(-1)^n) */
seq_t a(idx_t n)
{
    /* Handle the sequence for all n >= 1.  The OEIS offset is 1. */
    idx_t block = (n - 1) / 4;         /* number of completed 8‑number blocks   */
    idx_t pos   = (n - 1) % 4;         /* position within the current block    */

    /* Offsets for the four valid residues modulo 8: 0,1,2,7.  */
    static const seq_t offsets[4] = {
        (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)7
    };

    /* Compute the term using only integer operations; the product
       is performed in the type seq_t to avoid overflow for 128‑bit
       or arbitrary‑precision builds.  */
    seq_t term = (seq_t)8 * (seq_t)block + offsets[pos];
    return term;
}
