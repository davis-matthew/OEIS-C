/*
 * A022361  Fibonacci sequence beginning 0, 27.
 * https://oeis.org/A022361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*A000045(n)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair_rec(idx_t n, seq_t *f, seq_t *f0) {
    if (n == 0) {
        *f  = 0;
        *f0 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_rec(n >> 1, &a, &b);
    seq_t two_b = b + b;                /* 2*b */
    seq_t c     = a * (two_b - a);      /* a*(2*b - a) */
    seq_t d     = a * a + b * b;        /* a^2 + b^2 */
    if ((n & 1) == 0) {
        *f  = c;
        *f0 = d;
    } else {
        *f  = d;
        *f0 = c + d;
    }
}

seq_t a(idx_t n) {
    seq_t f, _unused;
    fib_pair_rec(n, &f, &_unused);
    return f * (seq_t)27;
}
