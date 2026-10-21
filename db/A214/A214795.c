/*
 * A214795  a(n) is the smallest k>=2 such that n divides Fibonacci(k-1)+21.
 * https://oeis.org/A214795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k >= 2 such that n divides Fibonacci(k-1)+21
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: addition modulo mod (both a and b are < mod) */
static inline idx_t add_mod(idx_t a, idx_t b, idx_t mod)
{
    /* mod > 0, and a,b < mod */
    if (a >= mod - b)
        return a - (mod - b);
    else
        return a + b;
}

seq_t a(idx_t n)
{
    idx_t mod = n;              /* n >= 1 */
    idx_t f_prev = 0;           /* Fib(0) */
    idx_t f_curr = (mod == 0) ? 0 : (1 % mod);  /* Fib(1) */
    const idx_t c21 = 21 % mod; /* 21 modulo n */

    /* m = k-1, start from 1 (k=2) */
    for (idx_t m = 1;; ++m)
    {
        idx_t f = f_curr;                     /* Fib(m) */
        idx_t fm21 = add_mod(f, c21, mod);    /* (Fib(m)+21) mod n */
        if (fm21 == 0)
            return (seq_t)(m + 1);           /* a(n) = k = m+1 */

        /* next Fibonacci number modulo n */
        idx_t next = add_mod(f_prev, f_curr, mod);
        f_prev = f_curr;
        f_curr = next;
    }
}
