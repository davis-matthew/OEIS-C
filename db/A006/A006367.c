/*
 * A006367  Number of binary vectors of length n+1 beginning with 0 and
 *          containing just 1 singleton.
 * https://oeis.org/A006367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + Fibonacci(n-3), a(0) = 1, a(1) = 0,
 *            with Fibonacci(-1) = 1, Fibonacci(0) = 0
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 43, u64 n <= 88, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p2, p1, f, g, t;
    idx_t i;
    if (n == 0) return 1;
    if (n == 1) return 0;
    p2 = 1;  /* a(i-2) */
    p1 = 0;  /* a(i-1) */
    f = 1;   /* Fibonacci(i-3), starting at Fibonacci(-1) */
    g = 0;   /* Fibonacci(i-2) */
    for (i = 2; i <= n; i++) {
        t = p1 + p2 + f;
        p2 = p1;
        p1 = t;
        t = f + g;
        f = g;
        g = t;
    }
    return p1;
}
