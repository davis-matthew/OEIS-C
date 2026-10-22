/*
 * A004696  a(n) = floor(Fibonacci(n)/3).
 * https://oeis.org/A004696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(A000045(n)/3), computed by tracking F(n) = 3*q(n) +
 *            r(n): q(n) = q(n-1) + q(n-2) + (r(n-1) + r(n-2)) / 3, r(n) =
 *            (r(n-1) + r(n-2)) mod 3, q(0) = 0, r(0) = 0, q(1) = 0, r(1) = 1;
 *            a(n) = q(n)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 50, u64 n <= 96, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q_prev = 0, q_curr = 0;
    unsigned r_prev = 0, r_curr = 1;
    if (n == 0) return 0;
    for (idx_t i = 2; i <= n; i++) {
        unsigned s = r_prev + r_curr;
        seq_t q_next = q_prev + q_curr + (seq_t)(s / 3);
        q_prev = q_curr;
        q_curr = q_next;
        r_prev = r_curr;
        r_curr = s % 3;
    }
    return q_curr;
}
