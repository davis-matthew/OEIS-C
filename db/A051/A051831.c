/*
 * A051831  a(n) = Fibonacci(prime(n)) mod prime(n), where prime(n) is the
 *          n-th prime.
 * https://oeis.org/A051831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(prime(n)) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 9417, u64 n <= 9417,
 *            u128 n <= 9417, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Fast doubling Fibonacci modulo m.
   Computes (F(n), F(n+1)) modulo m. */
static void fib_pair_mod(uint64_t n, uint64_t mod,
                         uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }

    uint64_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);   /* a = F(k), b = F(k+1) */

    /* c = F(2k)   = a * (2*b - a)  mod m
       d = F(2k+1) = a*a + b*b    mod m */
    uint64_t two_b = (b << 1) % mod;
    uint64_t two_b_minus_a;
    if (two_b >= a) two_b_minus_a = two_b - a;
    else             two_b_minus_a = two_b + mod - a;

    uint64_t c = (a * two_b_minus_a) % mod;
    uint64_t d = ((a * a) % mod + (b * b) % mod) % mod;

    if (n & 1) {    /* n is odd: return F(2k+1), F(2k+2) */
        *fn  = d;
        *fn1 = (c + d) % mod;   /* F(2k+2) = F(2k) + F(2k+1) */
    } else {        /* n is even: return F(2k), F(2k+1) */
        *fn  = c;
        *fn1 = d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* prime(n) is 1‑based */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t fn, fn1;
    fib_pair_mod(p, p, &fn, &fn1);
    return (seq_t)fn;
}
