/*
 * A183656  Number of (n+1) X 4 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 70*5^n + 200*4^n + 330*3^n + 388*2^n + 302
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; undefined for n < 1 */

    seq_t p2 = 1, p3 = 1, p4 = 1, p5 = 1, p6 = 1;

    for (idx_t i = 1; i <= n; ++i) {
        p2 *= 2;
        p3 *= 3;
        p4 *= 4;
        p5 *= 5;
        p6 *= 6;
    }

    seq_t res = 0;
    res += 6 * p6;
    res += 70 * p5;
    res += 200 * p4;
    res += 330 * p3;
    res += 388 * p2;
    res += 302;

    return res;
}
