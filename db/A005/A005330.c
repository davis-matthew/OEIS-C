/*
 * A005330  Certain subgraphs of a directed graph.
 * https://oeis.org/A005330
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-2} C(n-1,i) * p(n-1-i) * Sum_{j=0..n-2-i}
 *            (-1)^j*(n-1-i-j)/p(j), p(n) = Product_{k=1..n} (2^k-1). With
 *            S(m) = p(m)*Sum_{j=0..m-1} (-1)^j*(m-j)/p(j) and T(m) =
 *            p(m)*Sum_{j=0..m} (-1)^j/p(j): S(m) = (2^m-1)*(S(m-1)+T(m-1)),
 *            T(m) = (2^m-1)*T(m-1) + (-1)^m, S(0) = 0, T(0) = 1, a(n)...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;
    idx_t N = n - 1;
    seq_t S = 0, T = 1, pw = 1, c = 1, res = 0;
    for (idx_t m = 1; m <= N; m++) {
        pw = pw * 2;
        seq_t f = pw - 1;
        S = f * (S + T);
        T = f * T;
        if (m & 1) T -= 1; else T += 1;
        c = c * (seq_t)(N - m + 1) / (seq_t)m;
        res += c * S;
    }
    return res;
}
