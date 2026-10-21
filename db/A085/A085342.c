/*
 * A085342  Number of primes between phi(n) and n, where n is included in the
 *          count if it is a prime, while phi(n) is never included in the
 *          count even if it is a prime.
 * https://oeis.org/A085342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = pi(n) - pi(phi(n))
 * Range:     u8 n <= 2519, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ---------- prime sieve data (static, built on demand) ---------- */
static idx_t *primes        = NULL;      /* array of primes */
static size_t primes_count  = 0;         /* number of primes */
static idx_t  sieve_limit   = 0;         /* largest n that has been sieved */

/* Build (or extend) a prime sieve up to 'limit'.  The array of primes
   is regenerated each time the sieving range increases.  For the
   typical limits used (< 1e5) this is inexpensive. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate a temporary sieve array */
    uint8_t *sieve = (uint8_t*)malloc((size_t)(limit + 1));
    if (!sieve) exit(1);

    for (size_t i = 0; i <= (size_t)limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    size_t sq = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sq; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes and rebuild the prime list */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) ++cnt;

    idx_t *new_primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) new_primes[idx++] = (idx_t)i;

    if (primes) free(primes);
    primes        = new_primes;
    primes_count  = cnt;
    sieve_limit   = limit;
    free(sieve);
}

/* Count how many primes ≤ n.  The primes array must have been built
   up to at least n before calling this routine. */
static size_t pi_count(idx_t n)
{
    size_t cnt = 0;
    for (size_t i = 0; i < primes_count && primes[i] <= n; ++i)
        ++cnt;
    return cnt;
}

/* Compute Euler's totient φ(n) using the primes array.  The array
   must contain all primes up to √n (which is satisfied because we
   call ensure_sieve(n) before using this routine). */
static idx_t totient(idx_t n)
{
    idx_t result = n;
    idx_t temp   = n;
    for (size_t i = 0; i < primes_count &&
            (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)                /* remaining prime factor */
        result -= result / temp;
    return result;
}

/* ----------------------------------------------------------------
   Public function required by the OEIS harness.  Computes a(n) for
   n ≥ 1 as described: the number of primes in the interval (φ(n),
   n], with φ(n) excluded even if it is prime.  The function
   works correctly for all supported seq_t and idx_t sizes and
   keeps intermediate results in sufficiently wide types.
---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* safety for idx_t = 0 */
    ensure_sieve(n);                     /* primes up to n are needed */

    idx_t phi_n  = totient(n);
    size_t pi_n  = pi_count(n);
    size_t pi_phi = pi_count(phi_n);

    return (seq_t)(pi_n - pi_phi);
}
