/*
 * A181604  Twin primes ending in 3.
 * https://oeis.org/A181604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th prime p such that p % 10 == 3 and (p == 3 or
 *            prime(p-2))
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* the build supplies seq_t (unsigned integer type) and idx_t (uint64_t)   */
static idx_t sieve_limit = 0;      /* current upper bound of the sieve      */
static seq_t *twin_primes = NULL;  /* array of collected primes ending in 3 */
static size_t twin_count = 0;      /* number of primes stored in twin_primes */

/* Ensure that twin_primes contains at least n terms (n >= 1). */
static void ensure_twin_count(idx_t n)
{
    if (twin_count >= n || n == 0)
        return;

    /* Start at a modest limit and double until we have enough terms.    */
    idx_t limit = (sieve_limit == 0) ? 1000000 : sieve_limit * 2;

    while (1)
    {
        /* ---------- Sieve of Eratosthenes up to 'limit' ---------- */
        bool *is_prime = (bool *)malloc((size_t)(limit + 1));
        if (!is_prime) abort();                     /* insufficient memory */
        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = true;
        if (limit >= 0)
        {
            is_prime[0] = false;
            is_prime[1] = false;
        }
        for (idx_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;

        /* ---------- Collect primes that satisfy the condition ------- */
        size_t cap = (limit / 10) + 10;        /* a rough estimate      */
        seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!arr) abort();                     /* insufficient memory   */
        size_t len = 0;

        for (idx_t p = 3; p <= limit; ++p)
        {
            if (!is_prime[p] || (p % 10 != 3))
                continue;

            if (p == 3 || (p >= 5 && is_prime[p - 2]))
            {
                if (len == cap)
                {
                    cap *= 2;
                    arr = (seq_t *)realloc(arr, cap * sizeof(seq_t));
                    if (!arr) abort();         /* insufficient memory   */
                }
                arr[len++] = (seq_t)p;
            }
        }

        free(is_prime);

        /* Replace any previously stored array.  */
        if (twin_primes)
            free(twin_primes);
        twin_primes = arr;
        twin_count  = len;
        sieve_limit = limit;

        if (twin_count >= n)
            break;                               /* we have enough terms */
        limit *= 2;                               /* enlarge for next try */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the sequence, return 0 as a guard */
        return (seq_t)0;
    ensure_twin_count(n);
    return twin_primes[n - 1];
}
