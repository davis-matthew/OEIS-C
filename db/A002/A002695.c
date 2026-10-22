/*
 * A002695  P_n'(3), where P_n is n-th Legendre polynomial.
 * https://oeis.org/A002695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 * s(n), where s = A001003 satisfies (n+1)*s(n)
 *            = 3*(2n-1)*s(n-1) - (n-2)*s(n-2), s(0) = 1, s(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s2 = 1;  /* s(0) */
    seq_t s1 = 1;  /* s(1) */
    idx_t i;
    seq_t t;
    for (i = 2; i <= n; i++) {
        seq_t num = (seq_t)3 * (seq_t)(2 * i - 1) * s1 - (seq_t)(i - 2) * s2;
        seq_t cur = num / (seq_t)(i + 1);
        s2 = s1;
        s1 = cur;
    }
    if (n % 2 == 0)
        t = (seq_t)(n / 2) * (seq_t)(n + 1);
    else
        t = (seq_t)n * (seq_t)((n + 1) / 2);
    return t * s1;
}
