/*
 * A030132  Digital root of Fibonacci(n).
 * https://oeis.org/A030132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9430 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = digital_root(Fibonacci(n))
 * (The digital root equals the Fibonacci number modulo 9, with 0 mapped to 9,
 *  except for n = 0 where a(0) = 0.)
 */

#include <stdint.h>

/* Compute Fibonacci(n) modulo 9 using fast doubling (recursive). */
static void fib_mod_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_mod_fast(n >> 1, &a, &b);

    /* c = F(2k) = F(k) * [2*F(k+1) – F(k)]  */
    /* d = F(2k+1) = F(k)^2 + F(k+1)^2      */
    /* All computations are performed modulo 9. */
    seq_t sb = (b * 2) % 9;                /* 2*b mod 9 */
    seq_t sub = (sb + 9 - a) % 9;          /* (2*b - a) mod 9, avoiding negative */
    seq_t c = (a * sub) % 9;
    seq_t d = (a * a + b * b) % 9;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % 9;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t fn, _;
    fib_mod_fast(n, &fn, &_);
    /* fn is Fibonacci(n) modulo 9 */
    /* Digital root: if fn == 0 -> 9, otherwise fn itself */
    return (fn == 0) ? 9 : fn;
}
