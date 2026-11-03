/*
 * A003779  Number of spanning trees in P_5 x P_n.
 * https://oeis.org/A003779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{j=1..4} U_{n-1}(2 - cos(j*Pi/5)), U = Chebyshev
 *            polynomial of the 2nd kind, i.e. a(n) = N(U_{n-1}((4-phi)/2)) *
 *            N(U_{n-1}((5-phi)/2)) with N the norm of Z[phi], phi =
 *            (1+sqrt(5))/2; each factor follows U_k = t*U_{k-1} - U_{k-2},
 *            U_{-1} = 0, U_0 = 1, computed in Z[psi], psi = 1 - phi
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Norm of U_{n-1}(t/2) with t = c + psi; values kept as s + r*psi with s, r >= 0. */
static seq_t norm_factor(idx_t n, unsigned c) {
    seq_t s0 = 0, r0 = 0;   /* U_{-1} */
    seq_t s1 = 1, r1 = 0;   /* U_0 */
    idx_t k;
    for (k = 1; k < n; k++) {
        /* (c + psi)(s + r psi) = c s + r + (s + (c+1) r) psi, since psi^2 = psi + 1 */
        seq_t s2 = (seq_t)c * s1 + r1 - s0;
        seq_t r2 = s1 + (seq_t)(c + 1) * r1 - r0;
        s0 = s1; r0 = r1;
        s1 = s2; r1 = r2;
    }
    /* N(s + r psi) = s^2 + s r - r^2 */
    return s1 * s1 + s1 * r1 - r1 * r1;
}

seq_t a(idx_t n) {
    return norm_factor(n, 3) * norm_factor(n, 4);
}
