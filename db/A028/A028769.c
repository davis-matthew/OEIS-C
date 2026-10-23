/*
 * A028769  Nonsquares mod 56.
 * https://oeis.org/A028769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 56*floor((n-1)/44) + s_{((n-1) mod 44)+1} where s =
 *            {2,3,5,6,7,10,11,12,13,14,15,17,18,19,20,21,22,23,24,26,27,29,30,31,
 *            33,34,35,37,38,39,40,41,42,43,45,46,47,48,50,51,52,53,54,55}
 *            These are the 44 residues mod 56 that are NOT quadratic residues
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the defined offset. */
    if (n == 0)
        return 0;

    /* The 44 nonsquare residues modulo 56, in ascending order. */
    static const seq_t residues[44] = {
        2, 3, 5, 6, 7, 10, 11, 12, 13, 14,
        15, 17, 18, 19, 20, 21, 22, 23, 24, 26,
        27, 29, 30, 31, 33, 34, 35, 37, 38, 39,
        40, 41, 42, 43, 45, 46, 47, 48, 50, 51,
        52, 53, 54, 55
    };

    /* Zero‑based block index and position within the block. */
    idx_t block = (n - 1) / 44;
    idx_t pos   = (n - 1) % 44;

    /* Compute a(n) using the formula above. */
    return (seq_t)block * 56 + residues[pos];
}
