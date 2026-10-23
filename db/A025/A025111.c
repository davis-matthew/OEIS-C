/*
 * A025111  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (F(2), F(3), F(4), ...), t = (primes).
 * https://oeis.org/A025111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..k} F(i+1) * prime_{n-i+2}
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where k = floor((n+1)/2), F is the Fibonacci sequence   */
/* with F(0)=0,F(1)=1, and prime_j is the j‑th prime.      */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* --- prime cache -------------------------------------------------- */
static uint64_t *prime_cache = NULL;   /* 1‑based array: prime_cache[1] = 2 */
static size_t    prime_len   = 0;      /* number of stored primes */

/* Ensure prime_cache contains at least 'need' primes (indices 1..need). */
static void ensure_primes(size_t need)
{
    if (prime_len >= need)
        return;

    /* Upper bound for the nth prime (for n >= 6):  p_n <= n(ln n + ln ln n) */
    size_t limit;
    if (need < 6) {
        limit = 15;
    } else {
        double x = (double)need;
        limit = (size_t)(x * (log(x) + log(log(x))) + 10.0);
    }

    /* Sieve of Eratosthenes up to 'limit'. */
    unsigned char *is_prime = (unsigned char *)malloc(limit + 1);
    if (!is_prime)
        return; /* allocation failure – caller will later crash */

    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;
    size_t sq = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sq; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Allocate or enlarge the prime cache. */
    if (!prime_cache) {
        prime_cache = (uint64_t *)malloc((need + 1) * sizeof(uint64_t));
        if (!prime_cache) {
            free(is_prime);
            return;
        }
    } else {
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, (need + 1) * sizeof(uint64_t));
        if (!tmp) {
            free(is_prime);
            return;
        }
        prime_cache = tmp;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit && idx < need; ++i)
        if (is_prime[i]) {
            ++idx;
            prime_cache[idx] = i;
        }
    prime_len = idx;
    free(is_prime);
}

/* --- Fibonacci cache ---------------------------------------------- */
static seq_t *fib_cache = NULL; /* 1‑based array: fib_cache[1] = F(1) = 1 */
static size_t fib_len   = 0;    /* highest computed index (>=1) */

/* Ensure fib_cache contains Fibonacci numbers up to index 'need'. */
static void ensure_fib(size_t need)
{
    if (fib_len >= need)
        return;

    if (!fib_cache) {
        fib_cache = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        if (!fib_cache)
            return;
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len = 1;
    } else if (fib_cache[0] == (seq_t)0) {
        /* nothing to do, cache already contains F0 */
    }

    if (fib_len < 1) {
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len = 1;
    }

    if (fib_len < need)
        fib_cache = (seq_t *)realloc(fib_cache, (need + 1) * sizeof(seq_t));

    for (size_t i = fib_len + 1; i <= need; ++i)
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];

    fib_len = need;
}

/* --- sequence implementation --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* k = floor((n+1)/2) */
    idx_t k = (n + 1) / 2;

    /* We need primes up to index n+1 (j = n - i + 2, max when i=1). */
    ensure_primes((size_t)(n + 1));

    /* We need Fibonacci numbers up to index k+1 (s(k)=F(k+1)). */
    ensure_fib((size_t)(k + 1));

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t f = fib_cache[(size_t)(i + 1)];           /* F(i+1) */
        size_t p_idx = (size_t)(n - i + 2);             /* prime index */
        uint64_t p = prime_cache[p_idx];
        sum += f * (seq_t)p;
    }
    return sum;
}
