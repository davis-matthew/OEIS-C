/*
 * A023417  Numerator of n*(n-3)*(3*n^2-6*n+2)/(3*(n-1)*(n-2)).
 * https://oeis.org/A023417
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of n*(n-3)*(3*n^2-6*n+2)/(3*(n-1)*(n-2))
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 231, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _gcd_seq(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;  /* values before the offset are unspecified */

    seq_t N    = (seq_t)n;                         /* cast index to seq_t */
    seq_t num1 = N * (N - 3);                      /* n*(n-3) */
    seq_t num2 = 3 * N * N - 6 * N + 2;            /* 3*n^2-6*n+2 */
    seq_t T    = num1 * num2;                      /* numerator of the fraction */
    seq_t D    = 3 * (N - 1) * (N - 2);             /* denominator of the fraction */

    seq_t g    = _gcd_seq(T, D);                   /* reduce fraction */
    return T / g;                                 /* numerator of the reduced fraction */
}
