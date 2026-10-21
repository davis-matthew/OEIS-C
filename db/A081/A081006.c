/*
 * A081006  a(n) = Fibonacci(4n) - 1, or Fibonacci(2n+1)*Lucas(2n-1).
 * https://oeis.org/A081006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4n) - 1 (OEIS A081006)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast‑doubling Fibonacci helper */
static void fib_pair(idx_t k, seq_t *fk, seq_t *fk1)
{
    if (k == 0) {
        *fk  = (seq_t)0;
        *fk1 = (seq_t)1;
        return;
    }
    seq_t f, f1;
    fib_pair(k >> 1, &f, &f1);          /* compute F(k/2), F(k/2+1) */

    seq_t two_f1 = f1 + f1;
    seq_t c = f * (two_f1 - f);         /* F(2m)   = F(m) * (2*F(m+1) – F(m)) */
    seq_t d = f * f + f1 * f1;          /* F(2m+1) = F(m)^2 + F(m+1)^2 */

    if ((k & 1ULL) == 0) {
        *fk  = c;
        *fk1 = d;
    } else {
        *fk  = d;
        *fk1 = c + d;
    }
}

/* Sequence A081006 */
seq_t a(idx_t n)
{
    /* 4·n fits in idx_t for all expected n */
    idx_t four_n = n << 2;              /* 4*n */
    seq_t fn, fn1;
    fib_pair(four_n, &fn, &fn1);
    return fn - (seq_t)1;
}
