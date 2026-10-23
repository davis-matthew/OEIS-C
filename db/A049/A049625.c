/*
 * A049625  Congruent to 1, 2, 4, 6, 8 or 9 mod 11, but with 2 instead of 1.
 * https://oeis.org/A049625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049625.
   Numbers congruent to 1,2,4,6,8 or 9 (mod 11), but with the
   first occurrence of 1 replaced by 2.  Thus the first two terms
   are both 2, and thereafter the list continues with the usual
   residues 1,2,4,6,8,9 modulo 11.  The sequence can be generated
   in O(1) time by grouping the terms in blocks of six. */

static const seq_t residues[6] = {1, 2, 4, 6, 8, 9};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;                /* first term (replacement of 1) */

    /* Determine the block (group) and the offset within the block. */
    idx_t group = n / 6;                /* 0‑based block index */
    idx_t pos   = n % 6;                /* position inside block */

    seq_t base = (seq_t)group * 11;     /* base value for this block */

    if (pos == 0)                       /* first element of every block */
        return base + 1;                /* residue 1 (except for the very first block, handled above) */

    /* For the other positions use the normal residues. */
    return base + residues[pos];
}
