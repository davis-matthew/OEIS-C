/*
 * A047396  Numbers that are congruent to {1, 2, 6} mod 8.
 * https://oeis.org/A047396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 8*k - 2 if n % 3 == 0 (k = n/3) 8*k - 7 if n % 3 == 1
 *            (k = (n+2)/3) 8*k - 6 if n % 3 == 2 (k = (n+1)/3) } This
 *            reproduces the OEIS entry A047396: numbers congruent to {1,2,6}
 *            (mod 8)
 * Range:     u8 n <= 96, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t rem = n % 3;
    if (rem == 0) {
        idx_t k = n / 3;
        return (seq_t)8 * (seq_t)k - (seq_t)2;
    } else if (rem == 1) {
        idx_t k = (n + 2) / 3;
        return (seq_t)8 * (seq_t)k - (seq_t)7;
    } else { /* rem == 2 */
        idx_t k = (n + 1) / 3;
        return (seq_t)8 * (seq_t)k - (seq_t)6;
    }
}
