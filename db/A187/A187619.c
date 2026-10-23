/*
 * A187619  Sum of the differences of the parts in each Goldbach partition of
 *          2n, A187129(n) - A185297(n).
 * https://oeis.org/A187619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{p prime, p ≤ n, 2n-p prime} (n - p) (for n ≥ 2)
 * Range:     u8 n <= 38, u16 n <= 824, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the sum of the differences of the parts in each Goldbach partition of 2n. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for the prime sieve */
static size_t __prime_limit = 0;
static bool *__isprime_cache = NULL;

/* ensure that the sieve covers at least up to 2*n */
static void __ensure_prime_limit(idx_t n)
{
    idx_t max_needed = 2 * n;
    if ((size_t)max_needed <= __prime_limit)
        return;

    /* reallocate and recompute sieve */
    size_t new_size = (size_t)max_needed + 1;
    bool *new_cache = (bool*)malloc(new_size);
    if (!new_cache) exit(1);  /* allocation failure */

    /* initialize as prime */
    for (size_t i = 0; i < new_size; ++i)
        new_cache[i] = true;
    if (new_size > 0) new_cache[0] = false;
    if (new_size > 1) new_cache[1] = false;

    /* simple sieve of Eratosthenes */
    for (size_t p = 2; p * p <= max_needed; ++p) {
        if (new_cache[p]) {
            for (size_t j = p * p; j <= max_needed; j += p)
                new_cache[j] = false;
        }
    }

    /* free old cache and update pointers */
    if (__isprime_cache)
        free(__isprime_cache);
    __isprime_cache = new_cache;
    __prime_limit   = max_needed;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2, but return 0 safely */

    __ensure_prime_limit(n);

    seq_t sum = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!__isprime_cache[(size_t)p])
            continue;
        idx_t q = 2 * n - p;
        if (q < 2 || !__isprime_cache[(size_t)q])
            continue;
        seq_t diff = (seq_t)(n - p) * 2;   /* q - p = 2*(n - p) */
        sum += diff;
    }
    return sum;
}
