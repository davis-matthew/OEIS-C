/*
 * A010690  Period 2: repeat (1,9).
 * https://oeis.org/A010690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 9^n mod 10 (period 2: 1, 9, 1, 9, ...)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n & (idx_t)1) ? (seq_t)9 : (seq_t)1;
}
