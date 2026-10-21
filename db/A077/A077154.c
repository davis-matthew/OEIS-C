/*
 * A077154  Smallest k such that there are as many composite numbers less than
 *          n as there are between n and k.
 * https://oeis.org/A077154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077154:  a(n) = smallest k > n such that
 *                    #composite numbers < n  ==  #composite numbers in (n, k).
 * This holds iff
 *          C(k-1) = C(n) + C(n-1)
 * where C(x) is the number of composites ≤ x. */

#include <stdlib.h>
#include <stdbool.h>

/* Global sieve and cumulative composite counter */
static idx_t  sieve_limit   = 0;      /* maximum index currently computed */
static bool  *sieve_prime   = NULL;    /* true  -> prime, false -> composite */
static seq_t *composite_ip  = NULL;    /* C(x) for 0 ≤ x ≤ sieve_limit */

/* --- Ensure sieve and counter were built up to at least 'new_lim' */
static void
ensure_limit(idx_t new_lim)
{
    if (new_lim <= sieve_limit) return;

    /* Free old storage */
    if (sieve_prime)   free(sieve_prime);
    if (composite_ip)  free(composite_ip);

    /* Allocate new arrays with 1‑based safety */
    sieve_prime   = (bool *)malloc((new_lim + 1) * sizeof(bool));
    composite_ip  = (seq_t *)malloc((new_lim + 1) * sizeof(seq_t));
    if (!sieve_prime || !composite_ip) abort();

    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= new_lim; ++i) sieve_prime[i] = true;
    if (new_lim >= 0) sieve_prime[0] = false;
    if (new_lim >= 1) sieve_prime[1] = false;

    for (idx_t i = 2; i * i <= new_lim; ++i) {
        if (sieve_prime[i]) {
            for (idx_t j = i * i; j <= new_lim; j += i) {
                sieve_prime[j] = false;
            }
        }
    }

    /* Prefix sum of composites: composite_ip[x] = #composite ≤ x */
    composite_ip[0] = 0;
    for (idx_t i = 1; i <= new_lim; ++i) {
        composite_ip[i] = composite_ip[i-1]
                         + ((sieve_prime[i] == false && i > 1) ? 1 : 0);
    }

    sieve_limit = new_lim;
}

/* --- Main sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for the offset 1 */

    /* Need data up to at least n (for C(n)) */
    ensure_limit(n);

    /* C(n-1) and C(n) */
    seq_t C_nm1 = composite_ip[n-1];
    seq_t C_n   = composite_ip[n];

    /* Target value: C(k-1) must equal C(n) + C(n-1) */
    seq_t target = C_nm1 + C_n;

    /* Search for the smallest k > n with composite_ip[k-1] == target.
     * We test m = k (so m-1 used inside loop).  Start at n+1. */
    for (idx_t m = n + 1;; ++m) {
        if (m > sieve_limit)
            ensure_limit(sieve_limit * 2 + 1000);  /* expand the sieve */

        if (composite_ip[m - 1] == target)
            return (seq_t)m;
    }
}
