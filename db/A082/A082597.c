/*
 * A082597  Number of sets of consecutive primes whose arithmetic mean is an
 *          integer, the largest prime of a set is n-th prime.
 * https://oeis.org/A082597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082597: a(n) = the number of k∈[1,n] for which the sum of the last k primes
   (ending at the n‑th prime) is divisible by k.
   Implemented by caching a growing list of primes and their prefix sums.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Helper to estimate an upper bound for the n‑th prime (Rosser's theorem). */
static uint64_t prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;                /* < 15 is enough for first 5 primes */
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn))); /* p_n < n(log n + log log n) for n>=6 */
    return (uint64_t)(bound + 3.0);       /* safe integer bound */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 for safety */

    /* Static state: list of primes and prefix sums.  The arrays grow
       only when a larger n is requested. */
    static uint64_t *primes   = NULL;
    static seq_t   *pref     = NULL;
    static idx_t    prime_cnt = 0;   /* number of primes stored */

    /* Ensure we have at least n primes */
    if (prime_cnt < n)
    {
        /* Compute a sieve limit sufficient for the n‑th prime. */
        uint64_t limit = prime_upper_bound(n);

        /* Allocate and run simple Eratosthenes sieve. */
        bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
        if (!is_prime) return (seq_t)0;  /* allocation failure */

        for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = true;
        for (uint64_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;

        /* Count primes up to the limit. */
        size_t new_cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (is_prime[i]) ++new_cnt;

        /* Allocate or enlarge the primes array. */
        uint64_t *new_primes = (uint64_t*)realloc(primes, new_cnt * sizeof(uint64_t));
        if (!new_primes) { free(is_prime); return (seq_t)0; }
        primes = new_primes;

        /* Fill the primes array. */
        size_t idx = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (is_prime[i]) primes[idx++] = i;

        free(is_prime);

        /* Allocate or enlarge the prefix sums array. */
        seq_t *new_pref = (seq_t*)realloc(pref, (new_cnt + 1) * sizeof(seq_t));
        if (!new_pref) return (seq_t)0; /* memory error */
        pref = new_pref;

        /* Compute prefix sums: pref[0]=0, pref[i]=sum of first i primes. */
        pref[0] = (seq_t)0;
        for (size_t i = 1; i <= new_cnt; ++i)
            pref[i] = pref[i-1] + (seq_t)primes[i-1];

        prime_cnt = (idx_t)new_cnt;
    }

    /* Now compute the desired count. */
    seq_t total = pref[n];
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = total - pref[n - k];
        if ((sum % (seq_t)k) == 0)
            ++count;
    }
    return count;
}
