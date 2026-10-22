/*
 * A003731  Number of Hamiltonian cycles in C_5 X P_n.
 * https://oeis.org/A003731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) + 2*a(n-3) for n > 4, a(1) = 1, a(2)
 *            = 5, a(3) = 30, a(4) = 160
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x, y, z, t;
    idx_t i;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)30;
    if (n == 4) return (seq_t)160;
    x = (seq_t)5;    /* a(i-3) */
    y = (seq_t)30;   /* a(i-2) */
    z = (seq_t)160;  /* a(i-1) */
    for (i = 5; i <= n; i++) {
        t = z * 6;
        t -= y * 4;
        t += x * 2;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
