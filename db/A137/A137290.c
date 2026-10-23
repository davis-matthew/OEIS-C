/*
 * A137290  Fibonacci(n) mod 30.
 * https://oeis.org/A137290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) mod 30
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci(n) modulo 30 using fast doubling.  The algorithm
   works with the provided integer type seq_t and performs all
   intermediate arithmetic modulo 30 to keep numbers small. */
static void fib_mod_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_mod_pair(n >> 1, &a, &b);               /* (F(k), F(k+1)) */

    seq_t two_b = ((seq_t)2 * b) % ((seq_t)30); /* 2*b mod 30 */
    seq_t rm    = (two_b + (seq_t)30 - a) % ((seq_t)30); /* (2*b - a) mod 30 */
    seq_t c     = (a * rm) % ((seq_t)30);        /* F(2k)  */
    seq_t d     = (a * a + b * b) % ((seq_t)30);/* F(2k+1) */

    if ((n & 1) == 0) {      /* n even: (F(2k), F(2k+1)) */
        *fn  = c;
        *fn1 = d;
    } else {                 /* n odd:  (F(2k+1), F(2k+2)) */
        *fn  = d;
        *fn1 = (c + d) % ((seq_t)30);
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_mod_pair(n, &fn, &fn1);
    return fn;   /* Fibonacci(n) mod 30 */
}
