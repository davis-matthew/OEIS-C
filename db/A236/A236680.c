/*
 * A236680  Dimension of the space of spinors in n-dimensional real space.
 * https://oeis.org/A236680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-8) for n>=9, with a(1)=1, a(2)=2, a(3)=4, a(4)=4,
 *            a(5)=4, a(6)=4, a(7)=8, a(8)=8
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* base values for residues 1..8 (index 0 unused) */
    static const seq_t base[9] = {
        (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)4,
        (seq_t)4, (seq_t)4, (seq_t)8, (seq_t)8
    };

    /* Determine the block number and the residue within the block */
    idx_t k = (n - 1) / 8;          /* number of complete 8‑step blocks before n */
    int   r = (int)((n - 1) % 8) + 1; /* residue (1‑based) within a block */

    seq_t res = base[r];

    /* Multiply by 16^k by repeated multiplication (portable for all seq_t types) */
    for (idx_t i = 0; i < k; ++i)
        res *= (seq_t)16;

    return res;
}
