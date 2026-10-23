/*
 * A047551  Numbers that are congruent to {0, 1, 6, 7} mod 8.
 * https://oeis.org/A047551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   let k = (n-1)/4, r = (n-1)%4. a(n) = k*8 + {0,1,6,7}[r] for n >=
 *            1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but return 0 for safety */
    idx_t k = (n - 1) / 4;        /* number of complete 8‑block groups */
    idx_t r = (n - 1) % 4;        /* position within the current block */
    seq_t base = (seq_t)k * 8;    /* base value of the block */
    switch (r) {
        case 0: return base;       /* residue 0 mod 8 */
        case 1: return base + 1;   /* residue 1 mod 8 */
        case 2: return base + 6;   /* residue 6 mod 8 */
        case 3: return base + 7;   /* residue 7 mod 8 */
        default: return base;      /* unreachable, but keeps compiler happy */
    }
}
