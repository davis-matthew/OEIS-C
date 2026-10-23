/*
 * A038555  Derivative of n in base 3.
 * https://oeis.org/A038555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   a(n) = ternary derivative of n (adjacent digits summed mod 3)
 * Range:     u8 n <= 738, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    unsigned char d[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        d[len++] = tmp % 3;
        tmp /= 3;
    }
    seq_t res = 0;
    for (int i = len - 1; i > 0; --i) {
        int sum = (d[i] + d[i - 1]) % 3;
        res = res * 3 + sum;
    }
    return res;
}
