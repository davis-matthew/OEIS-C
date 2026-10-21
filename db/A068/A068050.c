/*
 * A068050  Number of values of k, 1<=k<=n, for which floor(n/k) is prime.
 * https://oeis.org/A068050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{p prime ≤ n} (floor(n/p) − floor(n/(p+1)))
 * Range:     u8 n <= 343, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache of primes */
static idx_t  *primes_arr  = NULL;   /* primes up to the last built limit */
static size_t  primes_cnt  = 0;      /* number of stored primes          */
static idx_t   primes_limit = 0;     /* highest n for which primes were built */

/* Build all primes up to and including 'limit' using a simple sieve.
   The primes are stored in the static array primes_arr, resizing as needed.
   For C++ compatibility we cast the result of malloc()/realloc(). */
static void build_primes_upto(idx_t limit)
{
    if (limit <= primes_limit)
        return;                       /* already sufficient */

    /* Allocate temporary boolean sieve */
    bool *sieve = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!sieve) abort();

    /* Initialise sieve: 0 and 1 are not prime */
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    /* Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (!sieve[i]) continue;
        for (idx_t j = i * i; j <= limit; j += i)
            sieve[j] = false;
    }

    /* Count primes and resize the primes array accordingly */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++new_cnt;

    idx_t *new_primes = (idx_t*)realloc(primes_arr, new_cnt * sizeof(idx_t));
    if (!new_primes) abort();
    primes_arr = new_primes;

    /* Fill the primes array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            primes_arr[idx++] = i;

    primes_cnt   = new_cnt;
    primes_limit = limit;

    free(sieve);
}

/* Compute a(n) : number of k with 1 ≤ k ≤ n such that floor(n/k) is prime */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1; terms below 1 are irrelevant */

    /* Ensure we have all primes up to n */
    build_primes_upto(n);

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes_arr[i];
        if (p > n) break;           /* primes_arr contains only primes ≤ n */

        seq_t max_k = n_seq / (seq_t)p;               /* floor(n/p)   */
        seq_t min_k = n_seq / (seq_t)(p + 1) + 1;      /* floor(n/(p+1)) + 1 */

        if (max_k >= min_k)
            count += (max_k - min_k + 1);
    }

    return count;
}
