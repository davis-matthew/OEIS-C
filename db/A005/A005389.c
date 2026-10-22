/*
 * A005389  Number of Hamiltonian circuits on 2n times 4 rectangle.
 * https://oeis.org/A005389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 10*a(n-2) - a(n-4), a(1) = 1, a(2) = 6, a(3) =
 *            37, a(4) = 236 (from G.f.: x*(1-2*x-x^2)/(1-8*x+10*x^2+x^4))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w = 1, x = 6, y = 37, z = 236; /* a(i-3), a(i-2), a(i-1), a(i) */
    idx_t i;
    if (n == 1) return w;
    if (n == 2) return x;
    if (n == 3) return y;
    for (i = 4; i < n; i++) {
        seq_t t = (seq_t)8 * z - (seq_t)10 * y - w;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
