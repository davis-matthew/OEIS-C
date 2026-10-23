/*
 * A047530  Numbers that are congruent to {0, 1, 3, 7} mod 8.
 * https://oeis.org/A047530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,1,3,7][(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1‑based; n >= 1. */
    idx_t block = (n - 1) / 4;                /* number of complete 8‑element blocks */
    idx_t rem   = (n - 1) % 4;                /* position inside the current block */

    seq_t result = (seq_t)block * (seq_t)8;   /* base value from full blocks */

    /* Add the residue offset according to the remainder. */
    switch (rem) {
        case 0:   /* 0 mod 8 */ break;
        case 1:   result += 1; break;          /* 1 mod 8 */
        case 2:   result += 3; break;          /* 3 mod 8 */
        case 3:   result += 7; break;          /* 7 mod 8 */
    }
    return result;
}
