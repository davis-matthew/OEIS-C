/*
 * A002889  Arrays of dumbbells.
 * https://oeis.org/A002889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (1+x)^3/((1-x)^3*(1-x-x^2)^4), offset 1 (a(n) = [x^(n-1)]
 *            of the g.f.). Computed by streaming the coefficients of (1+x)^3
 *            through three prefix-sum stages (1/(1-x)) and four
 *            Fibonacci-type stages (y(i) = x(i) + y(i-1) + y(i-2), i.e.
 *            1/(1-x-x^2)). Only additions of nonnegative values are used
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 25, u64 n <= 64, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int num[4] = {1, 3, 3, 1};
    seq_t p1[7], p2[7];
    seq_t x = 0;
    idx_t N, i;
    int j;

    if (n < 1) return 0;
    N = n - 1;

    for (j = 0; j < 7; j++) {
        p1[j] = 0;
        p2[j] = 0;
    }

    for (i = 0; i <= N; i++) {
        x = 0;
        if (i <= 3) x += num[i];
        for (j = 0; j < 3; j++) {
            seq_t y = p1[j] + x;
            p1[j] = y;
            x = y;
        }
        for (j = 3; j < 7; j++) {
            seq_t y = x + p1[j] + p2[j];
            p2[j] = p1[j];
            p1[j] = y;
            x = y;
        }
    }
    return x;
}
