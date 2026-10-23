/*
 * A047406  Numbers that are congruent to {4, 6} mod 8.
 * https://oeis.org/A047406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 1 - (-1)^n (OEIS A047406)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Offset is 1: behaviour for n < 1 is irrelevant, but handle it safely */
    if (n == 0) return 0;

    seq_t val = (seq_t)n * (seq_t)4;   /* 4*n */
    if ((n & 1) == 0) {                /* even n → subtract 2 */
        val -= (seq_t)2;
    }
    /* odd n → value remains 4*n (which is congruent to 4 mod 8) */
    return val;
}
