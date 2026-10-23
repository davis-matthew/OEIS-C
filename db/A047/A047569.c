/*
 * A047569  Numbers that are congruent to {0, 1, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6)+{0,1,4,5,6,7}[(n-1)%6]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The sequence consists of all non‑negative integers that are not
       congruent to 2 or 3 modulo 8.  The six admissible residues in
       increasing order are 0, 1, 4, 5, 6, 7. */
    idx_t q = (n > 0) ? (n - 1) / 6 : 0;   /* number of complete blocks of 8 */
    idx_t r = (n > 0) ? (n - 1) % 6 : 0;   /* position inside the block */
    seq_t base = (seq_t)8 * (seq_t)q;      /* start of the 8‑integer block */

    switch (r) {
        case 0: return base + 0;
        case 1: return base + 1;
        case 2: return base + 4;
        case 3: return base + 5;
        case 4: return base + 6;
        case 5: return base + 7;
        default: return base; /* unreachable */
    }
}
