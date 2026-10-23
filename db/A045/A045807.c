/*
 * A045807  7-ish numbers (end in 53, 71, 79, 97).
 * https://oeis.org/A045807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100*⌊(n-1)/4⌋ + {53,71,79,97}[ (n-1) mod 4 ]
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Residues modulo 100 that end in 53, 71, 79, or 97. */
    static const seq_t residues[4] = {53, 71, 79, 97};

    /* Adjust for 1‑based indexing of OEIS. */
    idx_t k = (n - 1) / 4;   /* number of complete 4‑step blocks */
    idx_t r = (n - 1) % 4;   /* remainder in {0,1,2,3} */

    return residues[r] + (seq_t)(100 * k);
}
