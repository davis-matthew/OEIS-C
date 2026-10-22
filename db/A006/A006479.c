/*
 * A006479  From variance of Fibonacci search.
 * https://oeis.org/A006479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   G.f. x^3*(1+x+x^2)/((1-x)*(1-x-x^2)^3), i.e. a(n) = d(n)
 *            convolved three times with Fibonacci: c(n) = d(n) + c(n-1) +
 *            c(n-2), b(n) = c(n) + b(n-1) + b(n-2), a(n) = b(n) + a(n-1) +
 *            a(n-2), where d(3) = 1, d(4) = 2, d(n) = 3 for n >= 5, d(n) = 0
 *            for n < 3, and all sequences are 0 for n < 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c1 = 0, c2 = 0, b1 = 0, b2 = 0, a1 = 0, a2 = 0;
    seq_t cur = 0;
    for (idx_t i = 0; i <= n; i++) {
        seq_t d;
        if (i < 3) d = 0;
        else if (i == 3) d = 1;
        else if (i == 4) d = 2;
        else d = 3;
        seq_t c = d + c1 + c2;
        seq_t b = c + b1 + b2;
        cur = b + a1 + a2;
        c2 = c1; c1 = c;
        b2 = b1; b1 = b;
        a2 = a1; a1 = cur;
    }
    return cur;
}
