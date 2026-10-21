/*
 * A083662  a(n) = a(floor(n/2)) + a(floor(n/4)), n > 0; a(0)=1.
 * https://oeis.org/A083662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2047, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A083662
 *  Formula: a(0)=1; a(n)=a(floor(n/2))+a(floor(n/4))  (n>0)
 *  Equivalent: a(n)=F(floor(log2(n))+3)  for n>0,
 *  where F(k) is the k‑th Fibonacci number (F(0)=0, F(1)=1).
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* compute floor(log2(n)) */
    idx_t tmp = n;
    idx_t bits = 0;
    while (tmp > 1) {
        tmp >>= 1;
        ++bits;
    }

    idx_t k = bits + 3;           /* Fibonacci index */

    /* precompute Fibonacci numbers up to k (k ≤ 66 for 64‑bit n) */
    static seq_t fib[70];
    static bool init = false;
    if (!init) {
        fib[0] = 0;
        fib[1] = 1;
        for (idx_t i = 2; i < 70; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        init = true;
    }

    return fib[k];
}
