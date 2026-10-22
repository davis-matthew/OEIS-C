/*
 * A004697  a(n) = floor(Fibonacci(n)/4).
 * https://oeis.org/A004697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(Fibonacci(n)/4), computed by carrying F(k) as
 *            4*q(k) + r(k) with 0 <= r(k) < 4: q(k+1) = q(k) + q(k-1) +
 *            floor((r(k)+r(k-1))/4), r(k+1) = (r(k)+r(k-1)) mod 4,
 *            q(0)=q(1)=0, r(0)=0, r(1)=1
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 50, u64 n <= 96, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q0 = 0, q1 = 0;
    int r0 = 0, r1 = 1;

    for (idx_t i = 0; i < n; i++) {
        int s = r0 + r1;
        seq_t nq = q0 + q1 + (seq_t)(s / 4);
        int nr = s % 4;
        q0 = q1;
        r0 = r1;
        q1 = nq;
        r1 = nr;
    }

    return q0;
}
