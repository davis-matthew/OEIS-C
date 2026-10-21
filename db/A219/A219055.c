/*
 * A219055  Number of ways to write n = p+q(3-(-1)^n)/2 with p>q and p, q,
 *          p-6, q+6 all prime.
 * https://oeis.org/A219055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 8702, u16 n <= 8702, u32 n <= 8702, u64 n <= 8702,
 *            u128 n <= 8702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A219055 – Number of ways to write n = p + q · (3−(−1)^n)/2
 *  with p>q, p,q,p−6,q+6 all prime (OEIS offset 1).          */

#include <stdlib.h>
#include <stdbool.h>

/* Global data for a simple lazy sieve */
static bool   *is_prime_arr = NULL;   /* prime flag array 0/1      */
static uint64_t *prime_list = NULL;   /* list of prime values      */
static size_t   prime_cnt  = 0;       /* number of primes found    */
static idx_t    sieve_limit = 0;      /* highest number sieved     */

/* ------------------------------------------------------------------ */
/* Ensure the sieve up to at least 'limit' (inclusive).               */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                     /* already enough ready      */

    /* free previous data */
    if (is_prime_arr) free(is_prime_arr);
    if (prime_list)   free(prime_list);

    /* build sieve array up to 'limit' */
    is_prime_arr = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime_arr) abort();     /* allocation failure       */

    /* mark all as prime (starting from 2) */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime_arr[i] = true;

    /* classic Eratosthenes sieve */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (is_prime_arr[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_arr[j] = false;

    /* collect prime list */
    size_t tmp_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i])
            ++tmp_cnt;

    prime_list = (uint64_t *)malloc(tmp_cnt * sizeof(uint64_t));
    if (!prime_list) abort();       /* allocation failure       */

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i])
            prime_list[k++] = (uint64_t)i;

    prime_cnt   = tmp_cnt;
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build                              */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined but safe       */

    /* need primes up to n+6 for the tests */
    /* avoid overflow on n+6 for very large n */
    idx_t limit = (n <= UINT64_MAX - 6) ? n + 6 : UINT64_MAX;
    ensure_sieve(limit);

    seq_t count = 0;
    idx_t multiplier = (n % 2 == 0) ? 1 : 2;          /* 1 for even, 2 for odd   */
    idx_t q_max = (n - 1) / (multiplier + 1);        /* ensure p > q             */

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t q = prime_list[idx];
        if (q > q_max)
            break;                                  /* q too large              */

        /* q+6 must be prime */
        idx_t qp6 = q + 6;
        if (qp6 > sieve_limit || !is_prime_arr[qp6])
            continue;

        /* compute p = n - multiplier*q   (p > q guaranteed by q_max) */
        idx_t p = n - multiplier * q;
        if (p <= q || p > sieve_limit || !is_prime_arr[p])
            continue;                            /* p must be prime   */

        /* p-6 must be prime and >= 2 */
        idx_t pm6 = p - 6;
        if (pm6 < 2)
            continue;
        if (pm6 > sieve_limit || !is_prime_arr[pm6])
            continue;

        ++count;
    }

    return count;
}
