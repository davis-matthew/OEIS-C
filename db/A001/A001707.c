/*
 * A001707  Generalized Stirling numbers, [n+5,5]_2.
 * https://oeis.org/A001707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(n+3,3), where U(m,j) = [m+2,j+2]_2 (unsigned 2-Stirling
 *            numbers of the first kind), U(m+1,j) = (m+2)*U(m,j) + U(m,j-1),
 *            U(0,0) = 1, U(0,j) = 0 for j > 0, U(m,-1) = 0; e.g.f. of U(.,3)
 *            is (-log(1-x))^3/(6*(1-x)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t u0 = 1, u1 = 0, u2 = 0, u3 = 0;
    idx_t m;
    for (m = 0; m < n + 3; m++) {
        seq_t c = (seq_t)m + 2;
        u3 = c * u3 + u2;
        u2 = c * u2 + u1;
        u1 = c * u1 + u0;
        u0 = c * u0;
    }
    return u3;
}
