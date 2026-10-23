/*
 * A151971  Numbers k such that k^2 - k is divisible by 21.
 * https://oeis.org/A151971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*((n-1)/4) + {0,1,7,15}[(n-1)%4]
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence consists of all integers k such that k² - k is divisible by 21,
       i.e. k ≡ 0,1,7,15 (mod 21).  For every block of 21 consecutive integers
       there are exactly four terms, so we can compute the n‑th term directly.  */
    idx_t m = n - 1;                /* zero based index */
    idx_t q = m / 4;                /* how many complete 21‑blocks are before k */
    idx_t r = m % 4;                /* residue index within the block */

    /* Ordered residues for the 21‑block: 0, 1, 7, 15 */
    static const seq_t offsets[4] = {0, 1, 7, 15};

    return (seq_t)21 * (seq_t)q + offsets[r];
}
