/*
 * A004282  a(n) = n*(n+1)^2*(n+2)^2/12.
 * https://oeis.org/A004282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+2,2)*C(n+2,3) = ((n+1)*(n+2)/2) * (n*(n+1)*(n+2)/6)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 137, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n, y = (seq_t)n + 1, z = (seq_t)n + 2;
    seq_t t, p;

    /* t = (n+1)(n+2)/2, dividing the even factor first */
    if (y % 2 == 0)
        t = (y / 2) * z;
    else
        t = y * (z / 2);

    /* p = n(n+1)(n+2)/6, dividing factors by 3 and 2 before multiplying */
    {
        seq_t f[3];
        int i;
        f[0] = x; f[1] = y; f[2] = z;
        for (i = 0; i < 3; i++) {
            if (f[i] % 3 == 0) { f[i] = f[i] / 3; break; }
        }
        for (i = 0; i < 3; i++) {
            if (f[i] % 2 == 0) { f[i] = f[i] / 2; break; }
        }
        p = f[0] * f[1] * f[2];
    }

    return t * p;
}
