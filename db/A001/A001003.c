/*
 * A001003  Schroeder's second problem (generalized parentheses); also called
 *          super-Catalan numbers or little Schroeder numbers.
 * https://oeis.org/A001003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   (n+1)*a(n) = (6*n-3)*a(n-1) - (n-2)*a(n-2) for n > 1, a(0) =
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x2 = 1; /* a(i-2) */
    seq_t x1 = 1; /* a(i-1) */
    idx_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        idx_t d = i + 1;
        seq_t q1 = x1 / (seq_t)d;
        seq_t q2 = x2 / (seq_t)d;
        long long r1 = (long long)(x1 % (seq_t)d);
        long long r2 = (long long)(x2 % (seq_t)d);
        long long t = (long long)(6 * i - 3) * r1 - (long long)(i - 2) * r2;
        long long tq = t / (long long)d; /* exact */
        seq_t v = q1 * (seq_t)(6 * i - 3);
        if (tq > 0) v += (seq_t)(unsigned long long)tq;
        v -= q2 * (seq_t)(i - 2);
        if (tq < 0) v -= (seq_t)(unsigned long long)(-tq);
        x2 = x1;
        x1 = v;
    }
    return x1;
}
