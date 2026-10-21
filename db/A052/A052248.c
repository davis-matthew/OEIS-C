/*
 * A052248  Greatest prime divisor of all composite numbers between p and next
 *          prime.
 * https://oeis.org/A052248
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = max{ largest_prime_factor(k) | prime(n) < k < prime(n+1)
 *            }
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Prime generation and smallest prime factor (SPF) tables              */
/* --------------------------------------------------------------------- */
static idx_t *primes = NULL;   /* array of primes, 0‑indexed          */
static size_t  primes_num = 0; /* number of primes stored              */
static uint32_t *spf = NULL;   /* SPF table, 0‑indexed               */

/* Build primes and SPF up to the given limit (inclusive).  The table is
   rebuilt each time; memory is reallocated if necessary.                 */
static void build_primes_and_spf(size_t limit)
{
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) return;                     /* allocation failure */

    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;

    /* free old tables if they exist */
    if (primes) free(primes);
    if (spf)   free(spf);

    primes   = NULL;
    primes_num = 0;
    size_t primes_cap = 0;

    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) { free(is_prime); return; }      /* allocation failure */

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            /* store prime */
            if (primes_num == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 128;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_num++] = i;

            spf[i] = (uint32_t)i;

            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (is_prime[j]) {
                        is_prime[j] = 0;
                        spf[j] = (uint32_t)i;
                    }
                }
            }
        } else if (spf[i] == 0) {   /* should not happen for composite < limit */
            spf[i] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* Estimate a safe bound for the (n+1)-th prime; use a simple linear factor
   that is safe for n ≤ 20000 (the expected maximal n in tests).           */
static size_t estimate_prime_limit(idx_t n)
{
    /* For n ≥ 6, p_n < n (ln n + ln ln n); here we use a slightly
       larger bound: n * 15, which is more than enough for n ≤ 20000.
       For very small n we just return a minimal constant. */
    if (n < 6) return 30;
    return (size_t)((n + 5) * 15);
}

/* Ensure that the prime list contains at least (indices 0 … n) inclusive,
   i.e. primes_num > n.  If not, rebuild the table with a larger limit.   */
static void ensure_primes(idx_t n)
{
    if (primes_num > (size_t)n) return;          /* already sufficient */

    size_t limit = estimate_prime_limit(n);
    while (1) {
        build_primes_and_spf(limit);
        if (primes_num > (size_t)n) break;
        limit *= 2;                              /* grow until enough */
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence A052248                                                */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* offset is 2; below that is undefined */

    /* Make sure we have primes up to index n (for prime(n+1)) */
    ensure_primes(n);

    idx_t p_start = primes[n - 1];   /* prime(n)   */
    idx_t p_end   = primes[n];       /* prime(n+1) */

    seq_t max_lpf = 0;

    for (idx_t k = p_start + 1; k < p_end; ++k) {
        /* Skip primes */
        if (spf[k] == (uint32_t)k) continue;   /* k is prime */

        /* Find largest prime factor of k using the SPF table */
        seq_t lpf = (seq_t)spf[k];
        idx_t temp = k;
        while (temp > 1) {
            idx_t f = (idx_t)spf[temp];
            if (f > lpf) lpf = f;
            temp /= f;
        }
        if (lpf > max_lpf) max_lpf = lpf;
    }

    return max_lpf;
}
