/*
 * A028788  Nonsquares mod 75.
 * https://oeis.org/A028788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 75 * ((n-1) / 53) + nonresidues[(n-1) % 53]
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nonresidues are the sorted positive integers that are *not* quadratic residues
   modulo 75. There are 53 of them in each block of 75. */

seq_t a(idx_t n)
{
    /* sorted non‑square residues modulo 75 (excluding 0) */
    static const seq_t nonresidues[53] = {
        2, 3, 5, 7, 8, 10, 11, 12, 13, 14, 15, 17, 18, 20, 22, 23,
        26, 27, 28, 29, 30, 32, 33, 35, 37, 38, 40, 41, 42, 43,
        44, 45, 47, 48, 50, 52, 53, 55, 56, 57, 58, 59, 60, 62,
        63, 65, 67, 68, 70, 71, 72, 73, 74
    };

    const idx_t per_period = 53;   /* number of non‑residues in a block of 75 */
    const idx_t period     = 75u;   /* modulus for the repeating pattern */

    idx_t zidx = n - 1u;           /* convert to zero‑based index */
    idx_t block = zidx / per_period;
    idx_t pos   = zidx % per_period;

    return (seq_t)(block * period + nonresidues[pos]);
}
