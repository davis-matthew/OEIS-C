/*
 * A000575  Tenth column of quintinomial coefficients.
 * https://oeis.org/A000575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*c(n) - 20*c(n-1) + 15*c(n-2) - 4*c(n-3), where c(j) =
 *            binomial(j+9, 9) and c(j) = 0 for j < 0; this is the coefficient
 *            of x^n in the G.f. (10-20*x+15*x^2-4*x^3)/(1-x)^10. The c(j) are
 *            found by repeated prefix sums, with no division
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 41, u64 n <= 566, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t acc[10];
    seq_t c0 = 0, c1 = 0, c2 = 0, c3 = 0; /* c(j), c(j-1), c(j-2), c(j-3) */
    idx_t j;
    int i;
    for (i = 0; i < 10; i++) acc[i] = 0;
    for (j = 0; j <= n; j++) {
        acc[0] = 1;
        for (i = 1; i < 10; i++) acc[i] += acc[i - 1];
        c3 = c2;
        c2 = c1;
        c1 = c0;
        c0 = acc[9];
    }
    return (10 * c0 + 15 * c2) - (20 * c1 + 4 * c3);
}
