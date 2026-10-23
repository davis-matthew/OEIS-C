/*
 * A047251  Numbers that are congruent to {1, 3, 4, 5} (mod 6).
 * https://oeis.org/A047251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/4) + [1,3,4,5][(n-1) mod 4]
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the numbers congruent to {1,3,4,5} mod 6, with a(1)=1. */
seq_t a(idx_t n) {
    /*  n >= 1  (offset is 1) */
    idx_t q = (n - 1) / 4;      /* number of complete 6‑blocks before the term   */
    idx_t r = (n - 1) % 4;      /* position inside the next block (0…3)           */
    seq_t res = (seq_t)q * 6;   /* start of the current block                     */
    switch (r) {
        case 0: res += 1; break;
        case 1: res += 3; break;
        case 2: res += 4; break;
        case 3: res += 5; break;
    }
    return res;
}
