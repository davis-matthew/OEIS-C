/*
 * A052414  Numbers without 6 as a digit.
 * https://oeis.org/A052414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d > 5 by d + 1 in base‑9 representation of
 *            (n – 1)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n-1, but avoid underflow when n==0 (should not happen due to offset) */
    idx_t k = n - 1;
    if (k == 0) return 0;          /* a(1) = 0 */

    /* base‑9 digits of k (least significant first) */
    unsigned int d[32];
    int len = 0;
    while (k > 0) {
        d[len++] = k % 9;
        k /= 9;
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned int digit = d[i];
        if (digit > 5) digit++;       /* shift digits 6,7,8 to 7,8,9 */
        res = res * (seq_t)10 + (seq_t)digit;
    }
    return res;
}
