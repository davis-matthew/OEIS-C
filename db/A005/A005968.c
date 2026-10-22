/*
 * A005968  Sum of cubes of first n Fibonacci numbers.
 * https://oeis.org/A005968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} F(i)^3, F = A000045 (F(0) = 0, F(1) = 1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t f_prev = 0, f_curr = 1; /* F(i-1), F(i) */
    for (idx_t i = 1; i <= n; i++) {
        sum += f_curr * f_curr * f_curr;
        if (i < n) {
            seq_t f_next = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = f_next;
        }
    }
    return sum;
}
