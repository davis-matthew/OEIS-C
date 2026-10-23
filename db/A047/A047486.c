/*
 * A047486  Numbers that are congruent to {0, 1, 3, 5, 7} mod 8.
 * https://oeis.org/A047486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/5) + {0,1,3,5,7}[ (n-1) % 5 ]
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  That is, a(n) is the n-th non‑negative integer congruent to 0,1,3,5, or 7 mod 8. */

seq_t a(idx_t n)
{
    /* Ensure that n >= 1; behaviour for n == 0 is undefined and ignored. */
    idx_t tmp   = n - 1;          /* 0‑based index within the sequence */
    idx_t block = tmp / 5;        /* number of complete size‑5 blocks */
    idx_t pos   = tmp % 5;        /* position inside the current block   */

    /* Offsets of the 5 admissible residues within each block of 8 integers. */
    static const seq_t off[5] = {0, 1, 3, 5, 7};

    seq_t base = (seq_t)block * 8;/* start of the current block */
    return base + off[pos];
}
