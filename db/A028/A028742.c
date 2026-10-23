/*
 * A028742  Nonsquares mod 29.
 * https://oeis.org/A028742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028742: Nonsquares mod 29
   a(n) = the nth positive integer that is not a quadratic residue modulo 29
   (offset 1, so a(1)=2).  The set of non‑residues in {1,…,28} is
   {2,3,8,10,11,12,14,15,17,18,19,21,26,27}.  Since 29 is prime there are
   14 such numbers per residue cycle.  For  n≥1,
   cycles = (n-1)/14,   r = (n-1)%14,
   a(n) = residues[r] + 29*cycles. */
#include <stdint.h>

static const uint8_t residues[14] = {
    2, 3, 8, 10, 11, 12, 14, 15, 17, 18, 19, 21, 26, 27
};

seq_t a(idx_t n)
{
    /* n is 1‑based */
    seq_t nn = (seq_t)n;
    seq_t cycles = (nn - 1) / 14;
    seq_t r = (nn - 1) % 14;
    /* compute result: residues[r] + 29*cycles */
    seq_t result = (seq_t)residues[(idx_t)r] + ((seq_t)29 * cycles);
    return result;
}
