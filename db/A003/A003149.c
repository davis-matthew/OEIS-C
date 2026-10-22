/*
 * A003149  a(n) = Sum_{k=0..n} k!*(n - k)!.
 * https://oeis.org/A003149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} k!*(n-k)!, evaluated by Horner's rule
 *            without division: P(0) = 1, P(m) = (n-m+1)*P(m-1) + m!, and then
 *            a(n) = P(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t P = 1;
    seq_t f = 1;
    idx_t m;
    for (m = 1; m <= n; m++) {
        f *= (seq_t)m;
        P = P * (seq_t)(n - m + 1) + f;
    }
    return P;
}
