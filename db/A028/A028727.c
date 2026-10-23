/*
 * A028727  Nonsquares mod 14.
 * https://oeis.org/A028727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 14*floor((n-1)/6) + v[(n-1)%6] where v = {3,5,6,10,12,13}
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Residues modulo 14 that are not quadratic residues */
    static const seq_t residues[6] = { (seq_t)3, (seq_t)5, (seq_t)6,
                                      (seq_t)10, (seq_t)12, (seq_t)13 };

    /* Convert index to 0‑based */
    idx_t zero = n - (idx_t)1;

    /* Which block of 14 the nth term sits in */
    seq_t block = (seq_t)(zero / (idx_t)6);

    /* Position within the block (0..5) */
    uint8_t pos = (uint8_t)(zero % (idx_t)6);

    /* Compute the term as base of the block plus the residue */
    return block * (seq_t)14 + residues[(int)pos];
}
