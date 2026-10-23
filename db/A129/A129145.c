/*
 * A129145  Alternately odd prime and odd composite numbers.
 * https://oeis.org/A129145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1)=A065091(n), a(2n)=A071904(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS N.B. A065091(n)= the n‑th odd prime, A071904(n)=the n‑th odd composite */

/* Implementation uses a growing sieve of odd numbers, storing the odd
   primes and odd composites in two dynamically allocated arrays.  The
   sieve is extended only when needed, ensuring correctness for any
   n>=1 while remaining time‑efficient for n up to at least 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* global cached data */
static bool *is_prime = NULL;     /* sieve flags (true=prime) */
static seq_t *primes  = NULL;     /* list of odd primes, 3,5,7,...     */
static seq_t *comps   = NULL;     /* list of odd composites, 9,15,21...*/
static size_t prime_cnt = 0, comp_cnt = 0;
static idx_t sieve_limit = 0;

/* extend the sieve and the prime/composite lists to at least `new_limit`
/// `new_limit` must be >= sieve_limit, otherwise the function does nothing */
static void extend_to(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;  /* nothing to do */

    /* free previous data */
    if (is_prime) free(is_prime);
    if (primes)  free(primes);
    if (comps)   free(comps);

    /* allocate and initialise sieve */
    is_prime = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!is_prime) return;
    for (idx_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
    if (new_limit >= 0) is_prime[0] = false;
    if (new_limit >= 1) is_prime[1] = false;

    /* simple sieve of Eratosthenes (only odd numbers chosen later) */
    for (idx_t p = 2; p * p <= new_limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= new_limit; m += p)
                is_prime[m] = false;

    /* count odd primes and composites up to new_limit */
    size_t p_cnt = 0, c_cnt = 0;
    for (idx_t n = 3; n <= new_limit; n += 2)
        if (is_prime[n])
            ++p_cnt;
        else
            ++c_cnt;          /* odd composite */

    /* allocate arrays */
    primes = (seq_t *)malloc(p_cnt * sizeof(seq_t));
    comps  = (seq_t *)malloc(c_cnt  * sizeof(seq_t));

    /* fill arrays */
    size_t pi = 0, ci = 0;
    for (idx_t n = 3; n <= new_limit; n += 2) {
        if (is_prime[n]) {
            primes[pi++] = (seq_t)n;
        } else {
            comps[ci++] = (seq_t)n;
        }
    }

    /* store counts and new limit */
    prime_cnt   = p_cnt;
    comp_cnt    = c_cnt;
    sieve_limit = new_limit;
}

/* public sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined below offset – return 0 */
        return 0;

    /* odd index → odd prime (k-th odd prime) */
    if (n & 1) {
        idx_t k = (n + 1) / 2;                /* position among odd primes */
        if ((size_t)k > prime_cnt) {
            /* increase bound until we have at least k primes */
            idx_t limit = sieve_limit ? sieve_limit * 2 : 20 * k + 10;
            while (prime_cnt < k) {
                extend_to(limit);
                if (prime_cnt < k)
                    limit *= 2;
            }
        }
        return primes[k - 1];
    }

    /* even index → odd composite (k-th odd composite) */
    else {
        idx_t k = n / 2;                      /* position among odd composites */
        if ((size_t)k > comp_cnt) {
            /* increase bound until we have at least k composites */
            idx_t limit = sieve_limit ? sieve_limit * 2 : 5 * k + 10;
            while (comp_cnt < k) {
                extend_to(limit);
                if (comp_cnt < k)
                    limit *= 2;
            }
        }
        return comps[k - 1];
    }
}
