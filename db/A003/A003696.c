/*
 * A003696  Number of spanning trees in P_4 X P_n.
 * https://oeis.org/A003696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = U_{n-1}(2) * U_{n-1}(2+sqrt(2)/2) * U_{n-1}(2-sqrt(2)/2),
 *            where U_k is the Chebyshev polynomial of the second kind: U_0(y)
 *            = 1, U_1(y) = 2y, U_k(y) = 2y*U_{k-1}(y) - U_{k-2}(y). This
 *            comes from tau(P_4 X P_n) = prod_{j=1..3} U_{n-1}(1 + mu_j/2),
 *            where mu_j = 2 - 2cos(j*Pi/4) are the nonzer...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p0, p1, p2;          /* U_k(2): 1, 4, 15, 56, ... */
    seq_t a0, b0, a1, b1, a2, b2; /* U_k(2+sqrt(2)/2) = A_k + B_k*sqrt(2) */
    idx_t k;

    if (n <= 1) return (seq_t)1;

    p0 = (seq_t)1; p1 = (seq_t)4;
    a0 = (seq_t)1; b0 = (seq_t)0;
    a1 = (seq_t)4; b1 = (seq_t)1;

    for (k = 2; k < n; k++) {
        p2 = (seq_t)4 * p1 - p0;
        /* (4 + sqrt2)(a1 + b1 sqrt2) - (a0 + b0 sqrt2) */
        a2 = (seq_t)4 * a1 + (seq_t)2 * b1 - a0;
        b2 = a1 + (seq_t)4 * b1 - b0;
        p0 = p1; p1 = p2;
        a0 = a1; b0 = b1;
        a1 = a2; b1 = b2;
    }

    return p1 * (a1 * a1 - (seq_t)2 * b1 * b1);
}
