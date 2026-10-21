/*
 * A068356  Numbers k such that bigomega(k) = tau(prime(k)+1) where tau(k) =
 *          A000005(k).
 * https://oeis.org/A068356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 953, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068356:
   numbers k such that bigomega(k) = tau(prime(k)+1)
   (bigomega = total number of prime factors with multiplicity, tau = divisor count)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_uint64.h"   /* factor_uint64() */
#include "../utils/sieve_primes.h"   /* sieve_primes() */

/* Maximum number of terms to cache (should be > expected maximum n during tests) */
#define MAX_TERMS 50000

/* Cached sequence values */
static seq_t results[MAX_TERMS];
static size_t result_count = 0;

/* Cached prime list and its size */
static uint64_t *prime_arr = NULL;
static size_t prime_cnt = 0;

/* Highest k that has been examined so far */
static idx_t max_checked_k = 0;

/* --------------------------------------------------------------------- */
/* Helper: total number of prime factors (with multiplicity) of n       */
static int bigomega_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(n, fac, exp, &cnt);
    int sum = 0;
    for (int i = 0; i < cnt; ++i) sum += exp[i];
    return sum;
}

/* --------------------------------------------------------------------- */
/* Ensure prime_arr holds at least 'need' primes (1‑based indexing)      */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* Rough upper bound for the nth prime: p_n < n (log n + log log n) for n >= 6
       add a safety margin */
    size_t limit = need * 20 + 100;
    free(prime_arr);
    sieve_primes(limit, (idx_t **)&prime_arr, &prime_cnt);
}

/* --------------------------------------------------------------------- */
/* Main access function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence defined for n ≥ 1 */

    /* Compute terms until we have at least n entries */
    while (result_count < (size_t)n) {
        ++max_checked_k;          /* next k to test */
        uint64_t k = (uint64_t)max_checked_k;

        /* bigomega(k) */
        int omega = bigomega_uint64(k);

        /* prime(k) : need k primes */
        ensure_primes((size_t)max_checked_k);
        uint64_t prime_k = prime_arr[max_checked_k - 1];

        /* tau(prime_k + 1) – compute via factor_uint64 */
        uint64_t p1 = prime_k + 1;
        uint64_t fac[20];
        int exp[20];
        int cnt = 0;
        factor_uint64(p1, fac, exp, &cnt);
        uint64_t tau = 1;
        for (int i = 0; i < cnt; ++i) tau *= (uint64_t)(exp[i] + 1);

        if (omega == (int)tau) {
            if (result_count < MAX_TERMS) {
                results[result_count++] = (seq_t)k;
            } else {
                /* In practice this should never happen with current limits */
                break;
            }
        }
    }
    return results[n - 1];
}
