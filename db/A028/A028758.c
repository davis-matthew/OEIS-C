/*
 * A028758  Nonsquares mod 45.
 * https://oeis.org/A028758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 45 * ⌊(n‑1)/33⌋ + r[(n‑1) mod 33], where r is the sorted
 *            list of residues mod 45 that are not quadratic residues:
 *            {2,3,5,6,7,8,11,12,13,14,15,17,18,20,21,22,23,24,26,27,28,29,30,32,33,35,37,38,39,41,42,43,44}
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t _a028758_residues[33] = {
    2, 3, 5, 6, 7, 8,
    11, 12, 13, 14, 15, 17,
    18, 20, 21, 22, 23, 24,
    26, 27, 28, 29, 30, 32,
    33, 35, 37, 38, 39, 41,
    42, 43, 44
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n<1 is irrelevant */
    idx_t block = (n - 1) / 33;
    idx_t pos   = (n - 1) % 33;
    seq_t val = (seq_t)block * 45 + (seq_t)_a028758_residues[pos];
    return val;
}
