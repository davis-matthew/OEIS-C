/*
 * A006373  Numbers of terms in expressions for coefficients of Euler-Lagrange
 *          tensors in terms of Riemann-Christoffel curvature tensor and two
 *          of its contractions (viz., the Ricci curvature tensor and the
 *          Riemann curvature scalar) for n-dimensional differentiable
 *          manifolds having a general linear connection.
 * https://oeis.org/A006373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 9*a(n-1) - 15*a(n-2) - 7*a(n-3); a(0) = 1, a(1) = 2, a(2)
 *            = 7
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 7;
    
    seq_t a0 = 1;
    seq_t a1 = 2;
    seq_t a2 = 7;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t tmp = 9 * a2 - 15 * a1 - 7 * a0;
        a0 = a1;
        a1 = a2;
        a2 = tmp;
    }
    
    return a2;
}
