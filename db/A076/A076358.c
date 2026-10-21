/*
 * A076358  a(n) = numerator(n!/phi(n!)).
 * https://oeis.org/A076358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator of n! / φ(n!) = product of all primes p ≤ n
 *            that do NOT divide any (q – 1) for q ≤ n
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 40, u64 n <= 70, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------------- */
/* Helper data used by the function.  All static so that multiple calls do   */
/* not recompute from scratch.                                               */
/* -------------------------------------------------------------------------- */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int *prime_list   = NULL;   /* primes ≤ current_max_n */
static size_t prime_count = 0;     /* number of stored primes      */
static int  *cancel_counts = NULL; /* how many times a prime divides some (p–1) */
static size_t cancel_size = 0;     /* allocated size of cancel_counts array   */
static int   current_max_n = 0;    /* largest n for which tables are built   */

/* -------------------------------------------------------------------------- */
/* Compute all primes ≤ limit using a simple sieve and store them in          */
/* prime_list.  cancel_counts array is resized to cover the new limit.        */
/* -------------------------------------------------------------------------- */
static void sieve_up_to(int limit)
{
    /* Sieve */
    int *is_prime = (int *)malloc((limit + 1) * sizeof(int));
    if (!is_prime) return;                      /* out‑of‑memory – caller will fail */

    for (int i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (int p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (int j = p * p; j <= limit; j += p)
                is_prime[j] = 0;

    /* Allocate/resize prime list */
    int *new_list = (int *)malloc((limit + 1) * sizeof(int));
    if (!new_list) { free(is_prime); return; }
    size_t new_count = 0;
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_list[new_count++] = i;

    free(prime_list);
    prime_list = new_list;
    prime_count = new_count;

    /* Allocate/resize cancel_counts */
    int *new_counts = (int *)malloc((limit + 1) * sizeof(int));
    if (!new_counts) { free(is_prime); return; }
    free(cancel_counts);
    cancel_counts = new_counts;
    cancel_size = limit + 1;
    memset(cancel_counts, 0, cancel_size * sizeof(int));

    current_max_n = limit;
    free(is_prime);
}

/* -------------------------------------------------------------------------- */
/* Main sequence function.                                                    */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;

    unsigned int limit = (unsigned int)n;   /* n is small (≤ 10000 in tests) */

    /* Build or extend tables if a larger n is requested */
    if ((int)limit > current_max_n)
        sieve_up_to((int)limit);
    else
        /* Clear previous cancellation data for this call */
        memset(cancel_counts, 0, ((size_t)limit + 1) * sizeof(int));

    /* For every prime p ≤ n, factor p – 1 and increment counters */
    for (size_t i = 0; i < prime_count; ++i)
    {
        int p = prime_list[i];
        if (p > (int)limit) break;

        int val = p - 1;
        int tmp = val;

        for (size_t j = 0; j < prime_count && (uint64_t)prime_list[j] * prime_list[j] <= val; ++j)
        {
            int q = prime_list[j];
            if (tmp % q == 0)
            {
                int exp = 0;
                while (tmp % q == 0)
                {
                    tmp /= q;
                    ++exp;
                }
                cancel_counts[q] += exp;
            }
        }
        if (tmp > 1)                 /* remaining prime factor */
            cancel_counts[tmp] += 1;
    }

    /* Assemble the numerator: product of primes that never canceled */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i)
    {
        int q = prime_list[i];
        if (q > (int)limit) break;
        if (cancel_counts[q] == 0)
            result *= (seq_t)q;
    }

    return result;
}
