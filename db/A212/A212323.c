/*
 * A212323  a(n) = 3^n - Fibonacci(n).
 * https://oeis.org/A212323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - Fibonacci(n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Fast doubling algorithm for Fibonacci (F(0)=0, F(1)=1) */
static seq_t fib_fast(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t a = (seq_t)0;  /* F(k)   */
    seq_t b = (seq_t)1;  /* F(k+1) */

    /* Highest power of two <= n */
    idx_t mask = 1ULL << (63 - __builtin_clzll(n));
    while (mask) {
        seq_t t1 = a * (((b << 1) - a));
        seq_t t2 = a * a + b * b;
        if (n & mask) {
            a = t2;
            b = t1 + t2;
        } else {
            a = t1;
            b = t2;
        }
        mask >>= 1;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t fibn = fib_fast(n);
    return pow3 - fibn;
}
