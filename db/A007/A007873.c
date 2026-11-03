/*
 * A007873  Indices of last windows of trapezoidal maps.
 * https://oeis.org/A007873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = J(n) + 2*W(n), where J(n) = (2^n - (-1)^n)/3 (Jacobsthal:
 *            J(1) = 1, J(n) = 2*J(n-1) + (-1)^(n-1)) and W(n) =
 *            Sum_{i=0..L-1} w_i * 4^(L-1-i), L = (n-5)/2 for odd n >= 5, W(n)
 *            = 0 otherwise; the digit word w is (10)^infinity for n == 1 (mod
 *            4) and 10(1100)^infinity for n == 3 (mod 4). Equiva...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int a007873_digit(idx_t n, idx_t i) {
    if (n % 4 == 1) return (i % 2 == 0) ? 1 : 0;
    if (i == 0) return 1;
    if (i == 1) return 0;
    return ((i - 2) % 4 < 2) ? 1 : 0;
}

seq_t a(idx_t n) {
    seq_t j = 1;
    seq_t w = 0;
    idx_t i, L;
    for (i = 2; i <= n; i++) {
        if (i % 2 == 1) j = 2 * j + 1;
        else j = 2 * j - 1;
    }
    if (n % 2 == 0 || n < 5) return j;
    L = (n - 5) / 2;
    for (i = 0; i < L; i++) {
        w = 4 * w + (seq_t)a007873_digit(n, i);
    }
    return j + 2 * w;
}
