/*
 * A024461  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Fibonacci numbers), t = (composite numbers).
 * https://oeis.org/A024461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..floor((n+1)/2)} Fib(i) * Composite(n+1-i)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Static cached data */
    static idx_t computed_upto = 0;
    static seq_t *fib_cache = NULL;        /* fib_cache[1] = 1, fib_cache[2] = 1 */
    static size_t fib_cache_cnt = 0;       /* highest index currently stored   */
    static uint64_t *comp_cache = NULL;    /* comp_cache[0] = 4 (first composite) */
    static size_t comp_cache_cnt = 0;
    static uint64_t *primes = NULL;        /* list of primes used for primality test */
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint64_t last_candidate = 3;    /* last number examined for primality */

    /* Increase cached size if necessary */
    if (n > computed_upto)
    {
        /* Ensure Fibonacci cache up to n */
        if (fib_cache == NULL)
        {
            fib_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 3));
            if (!fib_cache) abort();
            fib_cache[0] = 0;
            fib_cache[1] = 1;
            fib_cache[2] = 1;
            fib_cache_cnt = 2;
        }
        else if (fib_cache_cnt < n)
        {
            fib_cache = (seq_t *)realloc(fib_cache, sizeof(seq_t) * (n + 3));
            for (size_t i = fib_cache_cnt + 1; i <= n; ++i)
                fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
            fib_cache_cnt = n;
        }

        /* Ensure composite cache up to n */
        if (comp_cache == NULL)
        {
            comp_cache = (uint64_t *)malloc(sizeof(uint64_t) * (n + 3));
            if (!comp_cache) abort();
            comp_cache_cnt = 0;
        }
        else if (comp_cache_cnt < n)
        {
            comp_cache = (uint64_t *)realloc(comp_cache, sizeof(uint64_t) * (n + 3));
        }

        /* Prime list for primality testing */
        if (primes == NULL)
        {
            primes_cap = 16;
            primes = (uint64_t *)malloc(sizeof(uint64_t) * primes_cap);
            if (!primes) abort();
            primes[0] = 2;
            primes[1] = 3;
            primes_cnt = 2;
        }

        /* Generate composites until we have n of them */
        while (comp_cache_cnt < n)
        {
            uint64_t candidate = last_candidate + 1;
            bool is_prime = true;
            uint64_t limit = (uint64_t)int_sqrt(candidate);
            for (size_t pi = 0; pi < primes_cnt && primes[pi] <= limit; ++pi)
            {
                if (candidate % primes[pi] == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
            {
                if (primes_cnt == primes_cap)
                {
                    primes_cap *= 2;
                    primes = (uint64_t *)realloc(primes, sizeof(uint64_t) * primes_cap);
                }
                primes[primes_cnt++] = candidate;
            }
            else
            {
                comp_cache[comp_cache_cnt++] = candidate;
            }
            last_candidate = candidate;
        }

        computed_upto = n;
    }

    /* Compute the convolution sum */
    idx_t k = (n + 1) >> 1;         /* floor((n+1)/2) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t s = fib_cache[i];
        idx_t comp_idx = n + 1 - i;                /* 1‑based index into t */
        seq_t t = (seq_t)comp_cache[comp_idx - 1];  /* comp_cache[0]→t(1)=4 */
        sum += s * t;
    }
    return sum;
}
