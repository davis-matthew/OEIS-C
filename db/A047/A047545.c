/*
 * A047545  Numbers that are congruent to {0, 1, 3, 4, 7} mod 8.
 * https://oeis.org/A047545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047545: Numbers congruent to {0,1,3,4,7} modulo 8
   Formula: a(n) = 8 * ⌊(n-1)/5⌋ + offset[(n-1) mod 5]
   where offset = {0,1,3,4,7} */

/* compute the nth term of A047545 */
seq_t a(idx_t n)
{
    /* a(1) = 0, so we allow n == 0 to return 0 (undefined behaviour per OEIS) */
    if (n == 0) return (seq_t)0;

    /* zero‑based index */
    idx_t idx = n - 1;
    idx_t full = idx / 5;          /* number of complete blocks of 5 terms */
    idx_t rem  = idx % 5;          /* position inside the block */

    /* residues that are kept in one 8‑period block */
    static const seq_t offset[5] = { (seq_t)0, (seq_t)1, (seq_t)3,
                                     (seq_t)4, (seq_t)7 };

    /* compute the value using only integer arithmetic */
    return ((seq_t)8 * full) + offset[rem];
}
