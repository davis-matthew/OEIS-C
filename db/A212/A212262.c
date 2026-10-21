/*
 * A212262  a(n) = 3^n + Fibonacci(n).
 * https://oeis.org/A212262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + Fibonacci(n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   fib_pair_local, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci is computed via fast‑doubling in O(log n). */

#include "../utils/pow3.h"   /* seq_t pow3(idx_t exp); */
#include "../utils/fib_pair_local.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow3(n);        /* 3^n */
    seq_t fib_n, fib_n1;
    fib_pair_local(n, &fib_n, &fib_n1); /* fib_n = F_n */
    return pow_val + fib_n;
}
