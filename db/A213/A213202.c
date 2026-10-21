/*
 * A213202  Number of ways to write n = p + sum_{k=1..m}(-1)^(m-k)*p_k, where
 *          p is a Sophie Germain prime and p_k is the k-th prime.
 * https://oeis.org/A213202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213202: Number of ways to write n = p + Σ_{k=1..m} (-1)^(m-k) p_k,
   where p is a Sophie Germain prime (p and 2p+1 are prime) and p_k is the
   k-th prime.  The algorithm computes the alternating sums s_m = Σ_{k=1..m}
   (-1)^(m-k) p_k recursively as s_m = p_m - s_{m-1} and tests all
   representations with p = n - s_m.
   The prime sieve is built once up to a bound sufficient for all n ≤ 10000.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static idx_t sieve_limit = 0;          /* largest number up to which we have a sieve   */
static bool *sieve_is_prime = NULL;    /* true iff index is a prime                    */
static uint32_t *sieve_primes = NULL;  /* list of primes in increasing order          */
static size_t sieve_primes_cnt = 0;     /* number of primes stored                      */

/* Builds a sieve of Eratosthenes up to 'lim' inclusive (lim >= sieve_limit).
   The sieve is stored in global arrays. */
static void build_primes_up_to(idx_t lim)
{
    if (sieve_limit >= lim) return;

    free(sieve_is_prime);
    free(sieve_primes);

    sieve_is_prime = (bool*)malloc((size_t)(lim + 1));
    if (!sieve_is_prime) exit(EXIT_FAILURE);
    memset(sieve_is_prime, 1, (size_t)(lim + 1));
    if (lim >= 0) sieve_is_prime[0] = false;
    if (lim >= 1) sieve_is_prime[1] = false;

    for (idx_t p = 2; (idx_t)p * p <= lim; ++p)
        if (sieve_is_prime[p])
            for (idx_t x = p * p; x <= lim; x += p)
                sieve_is_prime[x] = false;

    size_t cnt = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (sieve_is_prime[i]) ++cnt;

    sieve_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!sieve_primes) exit(EXIT_FAILURE);
    size_t pos = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (sieve_is_prime[i]) sieve_primes[pos++] = (uint32_t)i;

    sieve_primes_cnt = cnt;
    sieve_limit = lim;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* We build the sieve up to a bound that is safely large for n ≤ 10000.
       The bound is 4*n + 100; this is more than sufficient for all known
       representations. */
    idx_t limit = n >= (idx_t)25000 ? n + 1000 : 4 * n + 100;
    build_primes_up_to(limit);

    seq_t count = 0;
    seq_t prev_sum = 0;
    seq_t cur_sum = 0;
    const seq_t n_minus_2 = (seq_t)n - 2;

    for (size_t idx = 0; idx < sieve_primes_cnt; ++idx) {
        seq_t pk = (seq_t)sieve_primes[idx];
        cur_sum = pk - prev_sum;          /* s_m = p_m - s_{m-1} */

        if (cur_sum <= n_minus_2) {       /* p_candidate = n - s_m >= 2 */
            idx_t p_candidate = (idx_t)((seq_t)n - cur_sum);
            /* 2 * p_candidate + 1 is guaranteed to be <= limit
               because p_candidate <= n and limit >= 4*n + 100.   */
            if (sieve_is_prime[p_candidate] &&
                sieve_is_prime[2 * p_candidate + 1]) {
                count += 1;
            }
        }
        prev_sum = cur_sum;
    }

    return count;
}
