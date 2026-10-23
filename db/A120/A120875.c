/*
 * A120875  Product of twin primes minus 1.
 * https://oeis.org/A120875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n) * (p_n + 2) - 1, where p_n is the lower member of
 *            the nth twin prime pair
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache for twin prime lower members */
static idx_t *twin_primes = NULL;     /* array of lower members of twin prime pairs */
static size_t twin_count = 0;          /* number of twin primes currently stored */
static idx_t current_limit = 0;        /* highest number sieved so far */

/* ensure that twin_primes[] holds at least n elements */
static void ensure_twin_primes(idx_t n)
{
    if (twin_count >= n)
        return; /* already ready */

    /* initial sieve limit: a rough upper bound for the n-th twin prime */
    idx_t limit;
    if (current_limit == 0) {
        /* for small n just use 100, otherwise scale linearly */
        limit = (n > 100) ? n * 15 : 100;
    } else {
        limit = current_limit * 2;
    }

    while (1) {
        current_limit = limit;                    /* update the global limit */

        /* allocate array for prime sieve */
        size_t sz = (size_t)(limit + 1);
        char *is_prime = (char *)malloc(sz);
        if (!is_prime)
            return; /* allocation failed (unlikely for requested limits) */

        /* initialize sieve */
        memset(is_prime, 1, sz);
        is_prime[0] = is_prime[1] = 0;

        /* Eratosthenes sieve */
        for (idx_t i = 2; (i * i) <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* collect twin prime lower members */
        size_t capacity = (size_t)(limit / 2 + 1);
        idx_t *new_twin = (idx_t *)malloc(capacity * sizeof(idx_t));
        size_t cnt = 0;
        for (idx_t i = 2; i + 2 <= limit; ++i) {
            if (is_prime[i] && is_prime[i + 2]) {
                if (cnt < capacity)
                    new_twin[cnt++] = i;
            }
        }

        free(is_prime); /* sieve array no longer needed */

        if (cnt >= n) {
            /* we have enough twin primes */
            free(twin_primes);
            twin_primes = new_twin;
            twin_count  = cnt;
            return;
        }

        /* not enough yet – grow the sieve limit and retry */
        free(new_twin);
        limit *= 2; /* double the range */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* the sequence starts at n = 1; behaviour for n=0 is arbitrary */

    ensure_twin_primes(n);
    idx_t p = twin_primes[n - 1];
    seq_t product = (seq_t)p * ((seq_t)p + 2);
    return product - 1;
}
