/*
 * A002302  Generalized tangent numbers.
 * https://oeis.org/A002302
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n-3), where d^n/dx^n tan(x) = Sum_k T(n,k) tan(x)^k.
 *            With T(n+1,k) = (k+1)*T(n,k+1) + (k-1)*T(n,k-1) and b(n) =
 *            T(n,n-1): a(n+1) = (n-1)*b(n) + (n-3)*a(n), b(n+1) = (n+1)! +
 *            (n-1)*b(n), a(3) = 2, b(3) = 8
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t av = 2, bv = 8, f = 6; /* a(3), b(3), 3! */
    idx_t m;
    if (n < 3) return 0;
    for (m = 3; m < n; m++) {
        seq_t na, nb;
        f *= (seq_t)(m + 1);
        na = (seq_t)(m - 1) * bv + (seq_t)(m - 3) * av;
        nb = f + (seq_t)(m - 1) * bv;
        av = na;
        bv = nb;
    }
    return av;
}
