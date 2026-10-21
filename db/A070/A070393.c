/*
 * A070393  a(n) = 6^n mod 13.
 * https://oeis.org/A070393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n mod 13
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of 6^n modulo 13 is 12.  Using the precomputed 12‐term cycle
 * gives an O(1) implementation. */
seq_t a(idx_t n) {
    static const seq_t period[12] = {1, 6, 10, 8, 9, 2, 12, 7, 3, 5, 4, 11};
    return period[n % 12];
}
