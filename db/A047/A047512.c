/*
 * A047512  Numbers that are congruent to {1, 2, 4, 6, 7} mod 8.
 * https://oeis.org/A047512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/5) + [1,2,4,6,7][(n-1) mod 5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t OFFSET[5] = {
    (seq_t)1,
    (seq_t)2,
    (seq_t)4,
    (seq_t)6,
    (seq_t)7
};

seq_t a(idx_t n)
{
    /* n is 1‑based; the sequence is defined for n >= 1 */
    if (n == 0) return (seq_t)0;   /* not used, but keeps the function well‑defined */

    idx_t m      = n - 1;           /* shift to 0‑based index */
    idx_t block  = m / 5;           /* number of complete blocks of 5 valid residues */
    idx_t offset = m % 5;           /* position inside the current block */

    /* 8 * block may overflow idx_t, but casting to seq_t before the
       multiplication guarantees we perform the arithmetic in the
       target type. */
    seq_t base   = (seq_t)block * (seq_t)8;

    return base + OFFSET[offset];
}
