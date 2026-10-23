/*
 * A047499  Numbers that are congruent to {3, 4, 5, 7} mod 8.
 * https://oeis.org/A047499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047499: numbers congruent to {3,4,5,7} modulo 8.  */
/*  a(n) = 8 * floor((n-1)/4) + offset[(n-1) mod 4],  offset = {3,4,5,7}  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n==0; guard for safety */
        return 0;

    /* offsets for the four residues in each 8‑number block */
    const seq_t offset[4] = { (seq_t)3, (seq_t)4, (seq_t)5, (seq_t)7 };

    idx_t previous = n - 1;                 /* n-1 fits in idx_t */
    idx_t pos      = previous % 4;          /* position within the block (0..3) */
    seq_t base     = (seq_t)( (previous / 4) * 8 );   /* 8 * floor((n-1)/4) */

    return base + offset[pos];
}
