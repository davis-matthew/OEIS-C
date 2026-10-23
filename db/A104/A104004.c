/*
 * A104004  Expansion of (1-x) * (1+x) / ((1-2*x)*(1-x-x^2)).
 * https://oeis.org/A104004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib_fast, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*2^n - Fib(n+3)
 * where Fib(k) is the k‑th Fibonacci number with Fib(0)=0, Fib(1)=1.
 * This gives the OEIS sequence A104004.
 */

#include "../utils/pow2.h"      /* pow2(idx_t n) -> 2^n in seq_t               */
#include "../utils/fib_fast.h"  /* fib_fast(idx_t n, seq_t *f, seq_t *f_next) */

seq_t a(idx_t n)
{
    /* 2^n as seq_t */
    seq_t two_n = pow2(n);

    /* 3*2^n */
    seq_t three_two_n = two_n * 3;

    /* Fibonacci(n+3) */
    seq_t f_n3;
    seq_t f_n4;                   /* we don't use f_n4 but fib_fast requires it */
    fib_fast(n + 3, &f_n3, &f_n4);

    /* a(n) = 3*2^n - Fib(n+3) */
    return three_two_n - f_n3;
}
