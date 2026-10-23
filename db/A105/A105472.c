/*
 * A105472  Next-to-last digit of n-th Fibonacci number in decimal
 *          representation, a(n) = 0 for n <= 6.
 * https://oeis.org/A105472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(Fibonacci(n)/10) mod 10 a(n) is the tens digit of
 *            the nth Fibonacci number. Fibonacci numbers start with F(0)=0,
 *            F(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* implementation uses fast doubling to compute Fibonacci(n) modulo 100,
   then extracts the tens digit. The sequence is periodic with period 300,
   but we do not hard‑code a table: the fast doubling algorithm works for
   any n. */

#include <stdint.h>   /* for consistent types */

#define MOD ((seq_t)100)   /* modulus 100 */

/* fast doubling: returns (F(n), F(n+1)) modulo MOD */
static void fib_mod100(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_mod100(n >> 1, &a, &b);                /* a = F(k), b = F(k+1), k = n/2 */

    /* compute F(2k) = a * (2*b - a)  modulo MOD */
    seq_t two_b     = (seq_t)2 * b;            /* 0 <= two_b < 200 */
    seq_t t_minus_a = (two_b + MOD - a) % MOD;
    seq_t c = ((unsigned long long)a * t_minus_a) % MOD;

    /* compute F(2k+1) = a^2 + b^2  modulo MOD */
    seq_t d = (((unsigned long long)a * a) + ((unsigned long long)b * b)) % MOD;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % MOD;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_mod100(n, &fn, &fn1);    /* compute F(n) mod 100 */
    return (fn / 10) % 10;       /* tens digit of Fibonacci(n) */
}
