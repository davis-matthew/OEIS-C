/*
 * A006320  Royal paths in a lattice.
 * https://oeis.org/A006320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (2 - x^2)*(1+x)^(n+2)*(2+x)^(n-1) for n >= 1, a(0)
 *            = 1 (Lagrange inversion of R = 1 + z*R*(1+R), R = g.f. of
 *            A006318). So a(n) = 2*c1 - c0, where c1 = Sum_{k=1..n}
 *            binomial(n+2,k)*binomial(n-1,k-1)*2^(k-1) and c0 =
 *            Sum_{k=0..n-2} binomial(n+2,k)*binomial(n-1,k+1)*2^(k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t c1 = 0;
    {
        seq_t bA = (seq_t)(n + 2); /* binomial(n+2, k) */
        seq_t bB = 1;              /* binomial(n-1, k-1) */
        seq_t p = 1;               /* 2^(k-1) */
        for (idx_t k = 1; k <= n; k++) {
            c1 += bA * bB * p;
            bA = bA * (seq_t)(n + 2 - k) / (seq_t)(k + 1);
            bB = bB * (seq_t)(n - k) / (seq_t)k;
            p *= 2;
        }
    }

    seq_t c0 = 0;
    if (n >= 2) {
        seq_t bA = 1;              /* binomial(n+2, k) */
        seq_t bB = (seq_t)(n - 1); /* binomial(n-1, k+1) */
        seq_t p = 2;               /* 2^(k+1) */
        for (idx_t k = 0; k + 2 <= n; k++) {
            c0 += bA * bB * p;
            bA = bA * (seq_t)(n + 2 - k) / (seq_t)(k + 1);
            bB = bB * (seq_t)(n - 2 - k) / (seq_t)(k + 2);
            p *= 2;
        }
    }

    return 2 * c1 - c0;
}
