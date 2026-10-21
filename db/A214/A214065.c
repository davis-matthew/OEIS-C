/*
 * A214065  a(n) is the least m > 0 such that Fibonacci(n+1+m) and n-m are not
 *          relatively prime.
 * https://oeis.org/A214065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least m > 0 such that gcd(Fibonacci(n+1+m), n-m) >
 *            1. The algorithm checks m=1,2,… until this condition holds,
 *            computing Fibonacci numbers modulo (n-m) using fast doubling
 * Range:     u8 n <= 386, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* modular multiplication using 128-bit to avoid overflow */
static inline unsigned long long mul_mod_u64(unsigned long long a,
                                            unsigned long long b,
                                            unsigned long long mod)
{
    return (unsigned long long)((__uint128_t)a * b % mod);
}

/* fast‑doubling algorithm that returns (F(n), F(n+1)) modulo mod */
static inline void fib_pair_u64(unsigned long long n,
                                unsigned long long mod,
                                unsigned long long *fn,
                                unsigned long long *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    unsigned long long a, b;
    fib_pair_u64(n >> 1, mod, &a, &b);          /* a=F(k), b=F(k+1) where k=n/2 */

    unsigned long long t2b = mul_mod_u64(2, b, mod);          /* (2*b) mod mod */
    unsigned long long t   = (t2b + mod - a) % mod;            /* (2*b - a) mod mod */
    unsigned long long c   = mul_mod_u64(a, t, mod);           /* F(2k) */
    unsigned long long d   = (mul_mod_u64(a, a, mod) +
                              mul_mod_u64(b, b, mod)) % mod;  /* F(2k+1) */

    if ((n & 1ULL) == 0) {      /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                    /* odd n */
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* returns F(n) mod mod */
static inline unsigned long long fib_mod_u64(unsigned long long n,
                                            unsigned long long mod)
{
    unsigned long long fn, fn1;
    fib_pair_u64(n, mod, &fn, &fn1);
    return fn;
}

seq_t a(idx_t n)
{
    idx_t nn = n;            /* keep a copy for subtraction */
    for (idx_t m = 1; ; ++m) {
        idx_t mod = nn - m;  /* n - m, never underflows because m <= n */

        if (mod == 0ULL) {   /* n - m == 0 -> gcd = Fibonacci(n+1+m) */
            /* Fibonacci(k) > 1 for k >= 3, and n+1+m >= n+2 >= 3 (n >= 1) */
            return (seq_t)m;
        }

        unsigned long long f_mod = fib_mod_u64(nn + 1 + m, mod);
        seq_t g = gcd((seq_t)f_mod, (seq_t)mod);
        if (g > 1) {
            return (seq_t)m;
        }
    }
}
