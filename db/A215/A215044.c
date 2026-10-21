/*
 * A215044  a(n) = F(2*n)^5 with F=A000045 (Fibonacci numbers).
 * https://oeis.org/A215044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(2*n)^5 where F(0)=0, F(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling implementation: computes F(n) with F(0)=0, F(1)=1.
   Uses only integer arithmetic (no external libraries). */
static void fib_fast(idx_t n, seq_t *fn) {
    if (n == 0) {
        *fn = 0;
        return;
    }
    seq_t a = 0;   /* F(k)   */
    seq_t b = 1;   /* F(k+1) */
    /* process bits from most significant to least significant */
    for (int i = 63; i >= 0; --i) {
        /* first compute F(2k) and F(2k+1) from F(k),F(k+1) */
        seq_t t = b << 1;            /* 2*b */
        t = t - a;                   /* 2*b - a */
        seq_t c = a * t;             /* F(2k)   = a*(2*b - a) */
        seq_t d = a * a + b * b;     /* F(2k+1) = a^2 + b^2   */
        if ((n >> i) & 1) {          /* next bit is 1: use odd index formula */
            a = d;                   /* F(2k+1) becomes new a */
            b = c + d;               /* F(2k+2) becomes new b */
        } else {                     /* next bit is 0: use even index formula */
            a = c;
            b = d;
        }
    }
    *fn = a;
}

seq_t a(idx_t n) {
    /* For n = 0 the Fibonacci term is F(0) = 0, so the result is 0. */
    if (n == 0) {
        return 0;
    }
    seq_t f;
    fib_fast(2 * n, &f);      /* f = F(2*n) */
    seq_t f2 = f * f;         /* f^2   */
    seq_t f4 = f2 * f2;       /* f^4   */
    return f4 * f;            /* f^5   */
}
