/*
 * A125627  Number of arithmetic progressions of primes < n-th prime.
 * https://oeis.org/A125627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 343, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of arithmetic progressions consisting of primes
 * that are all less than the n-th prime (OEIS A125627).
 * The first (n‑1) primes are considered; a(1)=0 because the set
 * is empty.  Progressions of any length ≥1 are counted, but an
 * arithmetic progression is counted only once, namely by its
 * smallest element and its common difference.
 *
 * The algorithm:
 *   * generate the first (n‑1) primes by a simple sieve,
 *   * count the trivial progressions of length 1 and 2,
 *   * for each pair of primes (p_i,p_j) with i<j:
 *        d = p_j−p_i
 *        if the progression can start earlier (p_i−d is prime) skip;
 *        otherwise extend the progression forward as far as possible
 *        within the set and add all subsequences of length ≥3.
 *
 * This is O(p^2) where p=n‑1 (~40000 for n=40000).  It runs well
 * below the 2 s limit for n up to about 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Global prime list and is‑prime bitmap.  They are regenerated
 * only when more primes are needed.  The memory lifetime is
 * the whole program lifetime (the problem environment never
 * frees it, which is fine). */
static uint32_t *primes   = NULL;  /* sorted primes, 0‑based */
static size_t   pcount   = 0;      /* number of primes stored   */
static char    *prime_map = NULL;  /* is‑prime bitmap, 0/1      */

/* Ensure that the first `need` primes are available.  The sieve
 * limit is chosen from a known upper bound for the n‑th prime. */
static void
ensure_primes_upto(idx_t need)
{
    if (pcount >= (size_t)need)
        return;                   /* already have enough */

    /* Upper bound for the n‑th prime (n ≥ 6)
     *  p_n < n(ln n + ln ln n)   (Rosser & Schoenfeld, 1962)
     *  For small n we use a conservative constant. */
    idx_t limit = (need < 6) ? 15 :
                  (idx_t)(need * (log((double)need) + log(log((double)need))) + 10);

    if (limit < 100)
        limit = 100;

    while (1) {
        /* Sieve of Eratosthenes */
        char *sieve = (char *)malloc((size_t)limit + 1);
        if (!sieve) abort();
        memset(sieve, 1, (size_t)limit + 1);
        sieve[0] = sieve[1] = 0;

        for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
            if (sieve[i]) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    sieve[j] = 0;
            }
        }

        /* Count primes */
        size_t cnt = 0;
        for (uint32_t i = 2; i <= limit; ++i)
            if (sieve[i]) ++cnt;

        if (cnt >= (size_t)need) {
            /* Store primes in a static array */
            uint32_t *tmp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
            if (!tmp) abort();
            size_t idx = 0;
            for (uint32_t i = 2; i <= limit; ++i)
                if (sieve[i]) tmp[idx++] = i;

            /* Free old structures */
            free(primes);
            free(prime_map);                /* memory is reused */

            primes   = tmp;
            prime_map = sieve;               /* reuse sieve bitmap */
            pcount   = cnt;
            return;
        }

        /* Not enough primes; enlarge the sieve limit */
        free(sieve);
        limit *= 2;
    }
}

/* Return 1 if x is prime, 0 otherwise.  x is guaranteed to be
 * <= last prime in current table. */
static inline int
is_prime_x(uint32_t x)
{
    return prime_map[x];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1)=0 by definition */

    /* Use the first (n‑1) primes. */
    idx_t m = n - 1;
    ensure_primes_upto(m);
    uint32_t last_prime = primes[m - 1];

    /* Count progressions of length 1 and 2 */
    seq_t result = (seq_t)m;               /* singles */
    result += (seq_t)m * (m - 1) / 2;      /* pairs */

    /* Count progressions of length ≥3 */
    for (idx_t i = 0; i < m; ++i) {
        uint32_t pi = primes[i];
        for (idx_t j = i + 1; j < m; ++j) {
            uint32_t pj = primes[j];
            uint32_t d  = pj - pi;          /* common difference */

            /* A progression starting at pi only if it can't start
             * earlier: pi - d must not be prime. */
            if (pi > d && is_prime_x(pi - d))
                continue;

            uint32_t cur = pi;
            uint32_t len = 1;
            while (cur + d <= last_prime && is_prime_x(cur + d)) {
                cur += d;
                ++len;
            }

            if (len >= 3) {
                /* For each progression of length len we have
                 * (len - 2)*(len - 1)/2 subsequences of
                 * length ≥3. */
                uint32_t l = len - 2;
                result += (seq_t)l * (len - 1) / 2;
            }
        }
    }

    return result;
}
