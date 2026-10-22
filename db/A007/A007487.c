/*
 * A007487  Sum of 9th powers.
 * https://oeis.org/A007487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)^2*(n^2+n-1)*(2*n^4+4*n^3-n^2-3*n+3)/20,
 *            computed as T^2*(n^2+n-1)*(2*n^4+4*n^3-n^2-3*n+3)/5 with T =
 *            n*(n+1)/2; the division by 5 is applied to whichever factor is
 *            divisible by 5, so no intermediate overflows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 105, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;
    unsigned r = (unsigned)(n % 5);

    seq_t t;
    if (n % 2 == 0) {
        t = (nn / 2) * n1;
    } else {
        t = nn * (n1 / 2);
    }

    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t f3 = n2 + nn - 1;
    seq_t f4 = 2 * n4 + 4 * n3 + 3 - n2 - 3 * nn;

    seq_t t1 = t, t2 = t;
    if (r == 0 || r == 4) {
        t1 = t / 5;
    } else if (r == 2) {
        f3 = f3 / 5;
    } else {
        f4 = f4 / 5;
    }

    return t1 * t2 * f3 * f4;
}
