/*
 * A000539  Sum of 5th powers: 0^5 + 1^5 + 2^5 + ... + n^5.
 * https://oeis.org/A000539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)^2*(2*n^2+2*n-1)/12 = T^2*(4*T-1)/3 with T =
 *            n*(n+1)/2. Divisions by 2 and 3 are taken from n or n+1 before
 *            computing in seq_t, so the result stays exact even when
 *            intermediate products wrap
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t x = n, y = n + 1;
    seq_t T, k;

    if (x % 2 == 0) x /= 2; else y /= 2;

    if (n % 3 != 1) {
        /* 3 divides T = x*y */
        seq_t T3;
        if (x % 3 == 0) x /= 3; else y /= 3;
        T3 = (seq_t)x * (seq_t)y;
        T = T3 * 3;
        return T3 * T * (4 * T - 1);
    }

    /* n = 3k+1: (4T-1)/3 = 6k^2 + 6k + 1 */
    T = (seq_t)x * (seq_t)y;
    k = (seq_t)(n / 3);
    return T * T * (6 * k * k + 6 * k + 1);
}
