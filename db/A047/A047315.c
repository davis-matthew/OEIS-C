/*
 * A047315  Numbers that are congruent to {2, 4, 5, 6} mod 7.
 * https://oeis.org/A047315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + [2,4,5,6][(n-1)%4] (for n >= 1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n < 1, return 0 */
    idx_t block = (n - 1) / 4;          /* number of complete 7‑number blocks */
    idx_t r     = (n - 1) % 4;          /* position within the block (0‑based) */
    seq_t base  = (seq_t)7 * (seq_t)block;  /* 7 * block, cast to seq_t first */
    seq_t offset;
    /* Map r to the corresponding remainder in {2,4,5,6} */
    switch (r) {
        case 0: offset = 2; break;
        case 1: offset = 4; break;
        case 2: offset = 5; break;
        default: offset = 6; /* r == 3 */
    }
    return base + offset;
}
