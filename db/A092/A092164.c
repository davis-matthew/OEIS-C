/*
 * A092164  Let M = 2 X 2 matrix [ 1 2 / 5 4 ]; a(n) = (1,1) entry of M^n.
 * https://oeis.org/A092164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*6^n + 5*(-1)^n)/7 Recurrence: a(0) = 1, a(1) = 1, a(n)
 *            = 5*a(n-1) + 6*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)1;        // a(0)
    seq_t a1 = (seq_t)1;        // a(1)
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)5 + a0 * (seq_t)6;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
