/*
 * A028740  Nonsquares mod 27.
 * https://oeis.org/A028740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 27 * floor((n‑1)/16) +
 *            {2,3,5,6,8,11,12,14,15,17,18,20,21,23,24,26}[(n‑1) mod 16],
 *            offset 1
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, return 0 */

    static const seq_t residues[16] =
        {2, 3, 5, 6, 8, 11, 12, 14, 15, 17, 18, 20, 21, 23, 24, 26};

    idx_t block = (n - 1) / 16;
    unsigned int pos = (unsigned int)((n - 1) % 16);

    return (seq_t)block * 27 + residues[pos];
}
