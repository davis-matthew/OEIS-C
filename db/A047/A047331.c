/*
 * A047331  Numbers that are congruent to {2, 3, 5, 6} mod 7.
 * https://oeis.org/A047331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/4) + {2,3,5,6}[ (n-1) mod 4 ]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the very first term for n >= 1.  Behaviour for n == 0 is irrelevant. */
    if (n == 0)
        return 0;

    /* Compute the number of complete 7‑length cycles that have been passed. */
    idx_t cycles = (n - 1) / 4;          /* integer division, 0‑based */
    idx_t r      = (n - 1) % 4;          /* remainder 0..3 */

    /* Offsets within each cycle for the residues 2,3,5,6 mod 7. */
    static const seq_t offsets[4] = {2, 3, 5, 6};

    /* Compute the value.  The cast to seq_t ensures the multiplication
     * is performed in the correct type (uint64_t or bigger). */
    seq_t base = (seq_t)7 * (seq_t)cycles + offsets[r];

    return base;
}
