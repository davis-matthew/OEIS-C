/*
 * A097136  a(n) = 3*Fibonacci(2*n) + 1.
 * https://oeis.org/A097136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Fibonacci(2*n) + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling Fibonacci. The function runs in O(log n) time. */

#include <stdint.h>

/* fast‑doubling: returns F(n) in *fn and F(n+1) in *fn1.
 * Uses only integer arithmetic on seq_t. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;          /* F(0) */
    seq_t b = 1;          /* F(1) */

    /* iterate over bits of n from most significant to least */
    for (int i = (sizeof(idx_t)*8) - 1; i >= 0; --i) {
        /* d = F(2k)  = F(k) * (2*F(k+1) - F(k)) */
        /* e = F(2k+1)= F(k)^2 + F(k+1)^2                 */
        seq_t t2b = b + b;                /* 2*F(k+1) */
        seq_t d = a * (t2b - a);
        seq_t e = a * a + b * b;

        if ((n >> i) & 1) {
            /* k -> 2k+1 */
            a = e;
            b = d + e;
        } else {
            /* k -> 2k   */
            a = d;
            b = e;
        }
    }

    *fn  = a;   /* F(n)   */
    *fn1 = b;   /* F(n+1) */
}

seq_t a(idx_t n)
{
    /* compute F(2*n) */
    idx_t two_n = n << 1;
    seq_t fn, fn1;
    fib_pair(two_n, &fn, &fn1);    /* fn = Fibonacci(2*n)   */

    /* a(n) = 3*fn + 1 */
    seq_t res = fn * (seq_t)3 + (seq_t)1;
    return res;
}
