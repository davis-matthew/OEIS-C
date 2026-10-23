/*
 * A034745  Dirichlet convolution of Fibonacci numbers with 3^(n-1).
 * https://oeis.org/A034745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} fib(d) * 3^(n/d-1) (OEIS A034745)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   fib, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the OEIS utilities fib.h and pow3.h for fib and 3^k computations. */

#include <stdlib.h>   /* malloc, realloc, free, exit */
#include <stdint.h>   /* uint64_t */
#include <stddef.h>   /* size_t */
#include "../utils/fib.h"
#include "../utils/pow3.h"

/* Cache of powers of 3: pow3_cache[e] = 3^e for 0 ≤ e ≤ pow3_len-1. */
static seq_t *pow3_cache = NULL;
static size_t pow3_len   = 0;   /* number of stored exponents */

/* Ensure the cache contains 3^e for all 0 ≤ e ≤ exp. */
static void ensure_pow3(size_t exp)
{
    if (exp < pow3_len) return;           /* already cached */

    /* Grow the array.  realloc returns void*, cast to seq_t*. */
    seq_t *new_cache = (seq_t *)realloc(pow3_cache, (exp + 1) * sizeof(seq_t));
    if (!new_cache) {                     /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }
    pow3_cache = new_cache;

    /* Extend the cache. */
    size_t old_len = pow3_len;
    pow3_len = exp + 1;   /* new length */
    for (size_t i = old_len; i <= exp; ++i) {
        if (i == 0)
            pow3_cache[0] = 1;                 /* 3^0 */
        else
            pow3_cache[i] = pow3_cache[i - 1] * 3;   /* multiply by 3 */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset 1 */

    seq_t sum = 0;

    /* Iterate over divisors i of n, using the symmetry i <-> n/i. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t q = n / i;            /* complementary divisor */

            /* Contribution of divisor i. */
            seq_t fi = fib(i);
            ensure_pow3((size_t)(q - 1));
            seq_t pow_q = pow3_cache[q - 1];
            sum += fi * pow_q;

            /* Contribution of divisor q if distinct from i. */
            if (i != q) {
                seq_t fq = fib(q);
                ensure_pow3((size_t)(i - 1));
                seq_t pow_i = pow3_cache[i - 1];
                sum += fq * pow_i;
            }
        }
    }
    return sum;
}
