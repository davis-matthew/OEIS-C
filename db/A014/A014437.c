/*
 * A014437  Odd Fibonacci numbers.
 * https://oeis.org/A014437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(3*floor((n+1)/2) + (-1)^n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1);

seq_t a(idx_t n) {
    idx_t k = (n + 1) / 2;      /* floor((n+1)/2) */
    idx_t idx;
    if ((n & 1) == 0) {
        idx = 3 * k + 1;        /* n even -> 3k+1 */
    } else {
        idx = 3 * k - 1;        /* n odd  -> 3k-1 */
    }
    seq_t fn, fn1;
    fib_pair(idx, &fn, &fn1);
    return fn;
}

/* Fast‑doubling algorithm for Fibonacci numbers.
   Computes a = F(n) and b = F(n+1).  All arithmetic is
   performed on seq_t; for unsigned types the modulo behaviour
   is well defined. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k),  b = F(k+1)  */
    seq_t two_b = b + b;               /* 2*F(k+1)   */
    seq_t c = a * (two_b - a);         /* F(2k)      */
    seq_t d = a * a + b * b;           /* F(2k+1)    */
    if (n & 1) {
        *fn  = d;                      /* odd n      */
        *fn1 = c + d;
    } else {
        *fn  = c;                      /* even n     */
        *fn1 = d;
    }
}
