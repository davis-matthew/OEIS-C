/*
 * A153483  a(n) = prime(n)^4 - 32.
 * https://oeis.org/A153483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n)^4 - 32
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 47, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* -------------- helper: estimate upper bound for the nth prime -------------- */
static idx_t upper_bound_nth_prime(idx_t n)
{
    /* For n >= 6, p_n <= n (ln n + ln ln n).  For smaller n use a small constant. */
    if (n < 6) return 15;             /* 6th prime is 13 */
    double dn = (double)n;
    double ln  = log(dn);
    double lnln = log(ln);
    double est = dn * (ln + lnln);
    return (idx_t)(est + 3.0);        /* extra room for safety */
}

/* -------------- helper: simple sieve to generate primes up to 'limit' -------------- */
static void sieve_primes(idx_t limit, idx_t **primes_out, size_t *prime_cnt)
{
    /* Allocate boolean array: true means composite */
    bool *sieve = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!sieve) { *primes_out = NULL; *prime_cnt = 0; return; }

    size_t max_cnt = (size_t)((double)limit / log((double)limit)) + 10;
    idx_t *primes = (idx_t*)malloc(max_cnt * sizeof(idx_t));
    if (!primes) { free(sieve); *primes_out = NULL; *prime_cnt = 0; return; }

    size_t count = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!sieve[p]) {
            if (count == max_cnt) {
                max_cnt *= 2;
                idx_t *tmp = (idx_t*)realloc(primes, max_cnt * sizeof(idx_t));
                if (!tmp) { free(sieve); free(primes); *primes_out = NULL; *prime_cnt = 0; return; }
                primes = tmp;
            }
            primes[count++] = p;
            if (p <= limit / p) {
                for (idx_t mult = p * p; mult <= limit; mult += p)
                    sieve[mult] = true;
            }
        }
    }
    free(sieve);
    *primes_out = primes;
    *prime_cnt = count;
}

/* -------------- static cache for primes -------------- */
static idx_t *prime_cache = NULL;
static size_t prime_cache_size = 0;   /* number of primes stored */

/* -------------- main sequence function -------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* out of range, undefined behaviour is allowed */

    /* Ensure the cache contains at least n primes */
    if ((size_t)n > prime_cache_size) {
        /* estimate needed limit */
        idx_t limit = upper_bound_nth_prime((idx_t)n);
        idx_t *new_cache = NULL;
        size_t new_cnt = 0;
        sieve_primes(limit, &new_cache, &new_cnt);
        if (!new_cache || new_cnt < (size_t)n) {
            /* Fallback: increase limit and resieve until enough primes */
            while (new_cnt < (size_t)n) {
                limit *= 2;
                free(new_cache);
                sieve_primes(limit, &new_cache, &new_cnt);
            }
        }
        /* free old cache */
        free(prime_cache);
        prime_cache = new_cache;
        prime_cache_size = new_cnt;
    }

    idx_t p = prime_cache[(size_t)n - 1];           /* nth prime */
    seq_t sp = (seq_t)p;                           /* promote to seq_t */
    seq_t sp2 = sp * sp;                           /* p^2 */
    seq_t sp4 = sp2 * sp2;                         /* p^4 */
    return sp4 - (seq_t)32;                        /* prime^4 - 32 */
}
