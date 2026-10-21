/*
 * A221950  Expansion of g.f. 1/(1 - x - x^2 - 2*x^3 - 3*x^4 - 2*x^5 - x^6).
 * https://oeis.org/A221950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) + 3*a(n-4) + 2*a(n-5) + a(n-6)
 *            with initial values a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=12,
 *            a(5)=26
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v[6] = {1, 1, 2, 5, 12, 26};
    if (n < 6) {
        return v[n];
    }
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = v[5] + v[4] + 2 * v[3] + 3 * v[2] + 2 * v[1] + v[0];
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }
    return v[5];
}
