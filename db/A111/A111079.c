/*
 * A111079  a(n) = gcd(F(n), product{k|n,k<n} F(k)), where F(k) is k-th
 *          Fibonacci number.
 * https://oeis.org/A111079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(F(n), ∏_{d|n, d<n} F(d))
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 47, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd_any, mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes F(k) via a simple Fibonacci recurrence and caches the
    sequence up to the largest n requested.  Modular arithmetic
    is used to keep the intermediate product within bounds. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd_any.h"
#include "../utils/mul128.h"

/* Cached Fibonacci numbers: fib_cache[0] = F(0), fib_cache[1] = F(1), … */
static seq_t *fib_cache = NULL;
static size_t fib_cache_size = 0;

/* Ensure fib_cache contains values F(0) … F(n). */
static void ensure_fib_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (fib_cache_size >= need) return;

    size_t new_size = need;
    seq_t *new_cache = (seq_t *)realloc(fib_cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);            /* allocation failure */

    /* Initialize base cases if the cache was empty. */
    if (fib_cache_size == 0) {
        new_cache[0] = (seq_t)0;
        if (new_size > 1) new_cache[1] = (seq_t)1;
    }

    /* Compute any new Fibonacci numbers.  Start at index 2. */
    size_t start = fib_cache_size;
    if (start < 2) start = 2;
    for (size_t i = start; i < new_size; ++i) {
        new_cache[i] = new_cache[i-1] + new_cache[i-2];
    }

    fib_cache     = new_cache;
    fib_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;         /* undefined below offset */

    ensure_fib_cache(n);
    seq_t Fn = fib_cache[(size_t)n];           /* F(n) */

    seq_t prod = (seq_t)1;                     /* product of F(d) (mod Fn) */

    /* Enumerate proper divisors of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d < n) {
                seq_t fd = fib_cache[(size_t)d];
                prod = mul128(prod, fd);
                if (prod >= Fn) prod %= Fn;
            }
            idx_t q = n / d;
            if (q != d && q < n) {
                seq_t fq = fib_cache[(size_t)q];
                prod = mul128(prod, fq);
                if (prod >= Fn) prod %= Fn;
            }
        }
    }

    return gcd_any(Fn, prod);                  /* gcd(Fn, product) */
}
