/*
 * A028743  Nonsquares mod 30.
 * https://oeis.org/A028743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Non-squares modulo 30.  The numbers that are not quadratic residues
    modulo 30.  The sequence is periodic with period 30 and contains
    exactly 18 residues in each block:
    {2,3,5,7,8,11,12,13,14,17,18,20,22,23,26,27,28,29}.  Hence,
    a(n) = 30 * floor((n-1)/18) + r[(n-1)%18]  where r[18] holds
    the sorted residues above. */
seq_t a(idx_t n)
{
    idx_t idx   = n - 1;               // zero‑based index
    idx_t block = idx / 18;            // how many complete periods before
    idx_t pos   = idx % 18;            // position inside the current period

    /* residues that are NOT squares modulo 30, sorted increasingly. */
    static const seq_t residues[18] = {
        2, 3, 5, 7, 8,
        11, 12, 13, 14, 17,
        18, 20, 22, 23, 26,
        27, 28, 29
    };

    return ((seq_t)block) * 30 + residues[pos];
}
