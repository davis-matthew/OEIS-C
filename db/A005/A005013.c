/*
 * A005013  a(n) = 3*a(n-2) - a(n-4), a(0)=0, a(1)=1, a(2)=1, a(3)=4.
 *          Alternates Fibonacci (A000045) and Lucas (A000032) sequences for
 *          even and odd n.
 * https://oeis.org/A005013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) for n even, a(n) = L(n) = 2*F(n+1) - F(n) for n odd
 *            (equivalently a(n) = 3*a(n-2) - a(n-4), a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=4)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 0, y = 1, t;
    idx_t i;
    for (i = 0; i < n; i++) {
        t = x + y;
        x = y;
        y = t;
    }
    /* now x = F(n), y = F(n+1) */
    if (n % 2 == 0) return x;
    return y + y - x;
}
