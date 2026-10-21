/*
 * A232896  a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4), where a(1) = 1, a(2)
 *          = 3, a(3) = 5, a(4) = 8.
 * https://oeis.org/A232896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) + A000045(n+1)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t two_b = b + b;                 /* 2*F(k+1) */
    seq_t c = a * (two_b - a);           /* F(2k)   */
    seq_t d = a * a + b * b;             /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* not used (offset 1) */
    seq_t fn_val, fn_next;
    fib_pair(n + 1, &fn_val, &fn_next);  /* fn_val = F(n+1) */
    seq_t result = fn_val + (seq_t)(n - 1);
    return result;
}
