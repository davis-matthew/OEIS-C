/*
 * A108021  Numbers n whose binary representation is the first Fibonacci(n)
 *          binary digits of the pattern 1010101010101010...
 * https://oeis.org/A108021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = binary number whose length is fib(n) and whose bits
 *            alternate 1,0,1,0,… starting with 1. (fib(1)=1, fib(2)=1, …)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute F(n) (the n‑th Fibonacci number) as a seq_t */
    seq_t f = 0;        /* F(0) */
    seq_t g = 1;        /* F(1) */
    if (n == 0) return 0;  /* defined only for n >= 1, but keeps function safe */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t tmp = f + g;   /* F(i+1) = F(i) + F(i-1) */
        f = g;               /* shift F(i) to F(i-1) position */
        g = tmp;             /* shift F(i+1) to F(i) position */
    }
    /* Now f = fib(n) */

    /* fib(n) fits into idx_t for n <= 10000 (fib(n) < 2^16) */
    idx_t len = (idx_t)f;

    seq_t val = 0;          /* the resulting number */
    seq_t bit = (seq_t)1;   /* first bit is 1 */

    for (idx_t i = 0; i < len; ++i) {
        val = (val << 1) | bit;
        bit ^= (seq_t)1;    /* toggle between 1 and 0 */
    }
    return val;
}
