/*
 * A047365  Numbers that are congruent to {0, 3, 4, 5} mod 7.
 * https://oeis.org/A047365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/4) + {0,3,4,5}[ (n-1) mod 4 ]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed >= 1 (the sequence offset is 1) */
    idx_t k = n - 1;                 /* zero‑based index */
    idx_t q = k / 4;                  /* number of complete 7‑blocks */
    uint8_t r = (uint8_t)(k % 4);     /* position inside the block (0..3) */

    /* offsets for r = 0,1,2,3  →  0,3,4,5  (mod 7) */
    static const seq_t off[4] = {0, 3, 4, 5};

    seq_t base = ((seq_t)q) * 7;      /* 7×complete blocks */
    return base + off[r];             /* add the block offset */
}
