/*
 * A085279  Expansion of (1 - 2*x - 2*x^2)/((1 - 2*x)*(1 - 3*x)).
 * https://oeis.org/A085279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t pow2 = (seq_t)1;     /* 2^0 */
    seq_t pow3prev = (seq_t)1; /* 3^(0) = 3^(n-1) when n==1 */
    seq_t res;
    for (idx_t i = 1; i <= n; ++i) {
        pow2 = pow2 * (seq_t)2;          /* compute 2^i */
        res = pow2 + pow3prev;           /* a(i) = 2^i + 3^(i-1) */
        if (i == n) return res;          /* return when reached target */
        pow3prev = pow3prev * (seq_t)3;  /* prepare 3^(i) for next iteration */
    }
    /* unreachable, but silence compiler warnings */
    return res;
}
