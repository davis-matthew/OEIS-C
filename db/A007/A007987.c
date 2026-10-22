/*
 * A007987  Number of irreducible words of length 2n in the free group with
 *          generators x,y such that the total degree of x and the total
 *          degree of y both equal zero.
 * https://oeis.org/A007987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002426(n)^2 - A002426(n-1)^2 for n > 0, a(0) = 1, where
 *            A002426 (central trinomial coefficients) satisfies k*T(k) =
 *            (2k-1)*T(k-1) + 3*(k-1)*T(k-2), T(0) = 1, T(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t0, t1, t2;
    idx_t k;
    if (n == 0) return 1;
    t0 = 1; /* T(0) */
    t1 = 1; /* T(1) */
    for (k = 2; k <= n; k++) {
        t2 = ((seq_t)(2 * k - 1) * t1 + (seq_t)(3 * (k - 1)) * t0) / (seq_t)k;
        t0 = t1;
        t1 = t2;
    }
    /* now t1 = T(n), t0 = T(n-1) */
    return t1 * t1 - t0 * t0;
}
