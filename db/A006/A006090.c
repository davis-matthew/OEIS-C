/*
 * A006090  Expansion of bracket function.
 * https://oeis.org/A006090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * Sum_{k=0..floor(n/6)} binomial(n+5,6*k+5),
 *            computed as (-1)^n * c_5(n+5), where c_r(m) = Sum_{j == r mod 6}
 *            binomial(m,j) satisfies c_r(m+1) = c_r(m) + c_{r-1 mod 6}(m),
 *            c_0(0) = 1, c_r(0) = 0 for r > 0. Every intermediate is a
 *            positive partial binomial row sum no larger than the...
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 28, i64 n <= 60, i128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c[6];
    seq_t t[6];
    int r;
    idx_t m;
    idx_t top = n + 5;
    seq_t result;

    for (r = 0; r < 6; r++) c[r] = 0;
    c[0] = 1;

    for (m = 0; m < top; m++) {
        for (r = 0; r < 6; r++) t[r] = c[r];
        for (r = 0; r < 6; r++) c[r] = t[r] + t[(r + 5) % 6];
    }

    result = c[5];
    if (n % 2 == 1) result = -result;
    return result;
}
