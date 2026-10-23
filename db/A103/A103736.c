/*
 * A103736  Fibonacci numbers with nonprime indices.
 * https://oeis.org/A103736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci number at the n-th nonprime index (indices 0
 *            and 1 are considered nonprime). The n-th nonprime index m is the
 *            (n‑th) element of the sequence of indices that are not prime.
 *            The value returned is F(m), the m‑th Fibonacci number with
 *            F(0)=0, F(1)=1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t *fib_arr = NULL;           /* Fibonacci numbers up to current limit */
static idx_t  current_limit = 0;        /* max index computed so far */
static idx_t *nonprime_idx = NULL;      /* m values for each n */
static seq_t *cache = NULL;             /* a(n) values (Fib at nonprime indices) */
static size_t cache_size = 0;           /* number of entries in cache */

/* Forward declaration of helper to extend precomputed tables */
static void ensure_capacity(idx_t n);

seq_t a(idx_t n)
{
    /* Ensure we have enough precomputed data for this n */
    if (!cache || n >= cache_size)
        ensure_capacity(n);

    return cache[n];
}

static void ensure_capacity(idx_t n)
{
    /* If already enough capacity, nothing to do */
    if (cache && (size_t)n < cache_size)
        return;

    /* Expand limit until we have at least n+1 nonprime indices */
    idx_t limit = current_limit ? current_limit : 64;
    while (1) {
        /* Allocate temporary array for primality status */
        char *is_prime = (char *)malloc((size_t)(limit + 1));
        if (!is_prime) abort();  /* allocation failure */
        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = 1;     /* prime flag initially true */

        /* 0 and 1 are not prime */
        if (limit >= 0) is_prime[0] = 0;
        if (limit >= 1) is_prime[1] = 0;

        /* Sieve of Eratosthenes */
        idx_t sqrt_lim = (idx_t) sqrt((double) limit);
        for (idx_t p = 2; p <= sqrt_lim; ++p) {
            if (is_prime[p]) {
                for (idx_t mult = p * p; mult <= limit; mult += p)
                    is_prime[mult] = 0;
            }
        }

        /* Count nonprime indices up to limit */
        size_t nonprime_count = 0;
        for (idx_t i = 0; i <= limit; ++i) {
            if (!is_prime[i])
                ++nonprime_count;
        }

        if (nonprime_count > (size_t)n) {
            /* We have enough nonprime indices; build data structures */
            /* Fibonacci numbers */
            seq_t *new_fib = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
            if (!new_fib) abort();
            new_fib[0] = (seq_t)0;
            if (limit >= 1) new_fib[1] = (seq_t)1;
            for (idx_t i = 2; i <= limit; ++i)
                new_fib[i] = new_fib[i - 1] + new_fib[i - 2];

            /* Nonprime indices list */
            idx_t *new_nonprime = (idx_t *)malloc(nonprime_count * sizeof(idx_t));
            if (!new_nonprime) abort();
            size_t idx = 0;
            for (idx_t i = 0; i <= limit; ++i) {
                if (!is_prime[i]) {
                    new_nonprime[idx++] = i;
                }
            }

            /* Cache for a(n): Fibonacci at nonprime indices */
            seq_t *new_cache = (seq_t *)malloc(nonprime_count * sizeof(seq_t));
            if (!new_cache) abort();
            for (size_t i = 0; i < nonprime_count; ++i)
                new_cache[i] = new_fib[new_nonprime[i]];

            /* Free old data */
            if (fib_arr) free(fib_arr);
            if (nonprime_idx) free(nonprime_idx);
            if (cache) free(cache);

            /* Assign new data */
            fib_arr = new_fib;
            nonprime_idx = new_nonprime;
            cache = new_cache;
            cache_size = nonprime_count;
            current_limit = limit;

            free(is_prime);
            break;
        }

        /* Not enough nonprime indices – increase limit and retry */
        free(is_prime);
        limit *= 2;   /* double the limit */
    }
}
