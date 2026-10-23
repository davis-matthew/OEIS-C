/*
 * A028775  Nonsquares mod 62.
 * https://oeis.org/A028775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 31*⌊(n-1)/15⌋ +
 *            [3,6,11,12,13,15,17,21,22,23,24,26,27,29,30][(n-1)%15]
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Non‑residue residues modulo 31 (sorted) */
    static const seq_t residues[15] = {
        (seq_t)3, 6, 11, 12, 13, 15, 17, 21, 22, 23, 24, 26, 27, 29, 30
    };

    /* 0‑based block and position in the residue list */
    idx_t block  = (n - 1) / 15;
    idx_t offset = (n - 1) % 15;

    /* Compute 31*block + residue */
    seq_t val = residues[offset] + (seq_t)31 * (seq_t)block;
    return val;
}
