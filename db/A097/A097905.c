/*
 * A097905  Triangle where T(n,m) = largest divisor of n! coprime to m.
 * https://oeis.org/A097905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): row‑wise flattening of triangle T(r,c) = largest divisor of r! coprime to c. */
/* For an index n (starting at 1), let r be the unique integer with r(r‑1)/2 < n ≤ r(r+1)/2, */
/* let c = n – (r‑1)r/2. Then a(n) = T(r,c).  The value is computed by multiplying the prime powers */
/* of r! for all primes p not dividing c, using Legendre's formula.  A simple prime list is maintained */
/* to support the calculation.  The implementation works for any seq_t/idx_t defined in the build environment. */

#include "../utils/big_pow.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime cache */
static uint64_t *g_primes = NULL;      /* array of primes ≤ current limit */
static size_t   g_prime_cnt  = 0;      /* number of primes stored    */
static size_t   g_prime_cap  = 0;      /* allocated capacity         */
static idx_t    g_prime_limit = 1;     /* highest integer tested for primality */

/* Ensure that all primes ≤ limit are stored in g_primes. */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_prime_limit)
        return;

    for (idx_t num = g_prime_limit + 1; num <= limit; ++num) {
        bool isprime = true;
        for (size_t i = 0; i < g_prime_cnt; ++i) {
            uint64_t p = g_primes[i];
            if (p * p > num)
                break;
            if (num % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (g_prime_cnt == g_prime_cap) {
                size_t new_cap = (g_prime_cap == 0) ? 16 : g_prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                g_primes = tmp;
                g_prime_cap = new_cap;
            }
            g_primes[g_prime_cnt++] = num;
        }
    }
    g_prime_limit = limit;
}

/* Return the n-th element of the triangle (flattened row-wise). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined; return 0 for safety */

    /* Compute row number r and column c from index n. */
    idx_t r = 1;
    idx_t total = 1;                /* elements up to current row r */
    while (total < n) {
        ++r;
        total += r;
    }
    idx_t prev_total = total - r;   /* elements before row r */
    idx_t c = n - prev_total;       /* 1‑based column within row r */

    /* Make sure the primes list covers all primes ≤ r. */
    ensure_primes(r);

    /* Compute the desired divisor. */
    seq_t result = 1;
    for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= r; ++i) {
        uint64_t p = g_primes[i];
        if (c % p == 0)
            continue;          /* prime divides c → ignore */

        /* Exponent of p in r! (Legendre's formula). */
        idx_t t = r;
        idx_t exp = 0;
        while (t) {
            t /= p;
            exp += t;
        }
        seq_t pow_val = big_pow((seq_t)p, (seq_t)exp);
        result *= pow_val;
    }

    return result;
}
