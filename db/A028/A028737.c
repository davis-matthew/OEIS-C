/*
 * A028737  Nonsquares mod 24.
 * https://oeis.org/A028737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028737 – Nonsquares modulo 24
   a(n) is the n-th positive integer that is NOT a quadratic residue modulo 24.
   The nonsquare residues modulo 24 are
   2,3,5,6,7,8,10,11,13,14,15,17,18,19,20,21,22,23,
   which form a fixed 24‑periodic pattern.  Therefore
   a(n) = 24 · floor((n-1)/18) + r[(n-1) mod 18], where r is the above list. */

static const seq_t residues[18] = {
    2, 3, 5, 6, 7, 8, 10, 11, 13, 14, 15, 17, 18, 19, 20, 21, 22, 23
};

seq_t a(idx_t n)
{
    idx_t i = n - 1;                 /* zero‑based index */
    idx_t block = i / 18;            /* number of full 24‑blocks before the term */
    idx_t offset = i % 18;           /* position within the block */
    return (seq_t)block * 24 + residues[offset];
}
