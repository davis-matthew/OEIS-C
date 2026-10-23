/*
 * A014694  a(n) = prime(n+1) - (-1)^n*ceiling(n/2).
 * https://oeis.org/A014694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+1) - (-1)^n*ceil(n/2)
 * Range:     u8 n <= 50, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached primes: prime_cache[0] = 2 = prime(1) */
static idx_t *prime_cache = NULL;
static size_t prime_cache_count = 0;      /* number of primes stored */
static size_t prime_cache_capacity = 0;   /* allocated slots */
static idx_t next_candidate = 2;          /* next integer to test */

/* Ensure prime_cache contains at least cnt primes */
static void
ensure_primes_count(idx_t cnt)
{
    while (prime_cache_count < (size_t)cnt) {
        /* test next_candidate for primality */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cache_count; ++i) {
            idx_t p = prime_cache[i];
            /* If p > sqrt(next_candidate) stop */
            if (p > next_candidate / p) {
                break;
            }
            if (next_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            /* grow cache if needed */
            if (prime_cache_count == prime_cache_capacity) {
                size_t new_cap = prime_cache_capacity ? prime_cache_capacity * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_cache = tmp;
                prime_cache_capacity = new_cap;
            }
            prime_cache[prime_cache_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* we need prime(n+1), i.e. prime_cache[n] since cache is 0‑based */
    ensure_primes_count(n + 1);
    seq_t prime_val = (seq_t)prime_cache[n];

    /* ceil(n/2) */
    seq_t half = (seq_t)((n + 1) / 2);

    if ((n & 1) == 0) /* n even, subtract */
        return prime_val - half;
    else              /* n odd, add */
        return prime_val + half;
}
