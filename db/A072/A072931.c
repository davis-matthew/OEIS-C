/*
 * A072931  Number of ways to write n as a sum of 2 semiprimes.
 * https://oeis.org/A072931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, look, nonn
 * Formula:   a(n) = number of ways to write n as a sum of two semiprimes. The
 *            implementation first extends a cache of smallest prime factors
 *            up to n, marks which numbers are semiprime (Omega == 2), and
 *            then counts the pairs i, n-i with i <= n/2. The array is grown
 *            only when a larger n is requested, so repeate...
 * Range:     u8 n <= 3779, u16 n <= 9956, u32 n <= 9956, u64 n <= 9956,
 *            u128 n <= 9956, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>     /* sqrt */

/* Cache state */
static idx_t max_precomp = 0;
static uint8_t *semiprime_cache = NULL;   /* 1 if semiprime, 0 otherwise */
static idx_t *spf_cache       = NULL;     /* smallest prime factor */

static void
ensure_precomp(idx_t n)
{
    if (n <= max_precomp)
        return;

    idx_t new_max = n;

    /* Allocate new semiprime cache */
    uint8_t *new_semiprime = (uint8_t *)malloc((new_max + 1) * sizeof(uint8_t));
    if (!new_semiprime)
        abort();
    if (semiprime_cache) {
        memcpy(new_semiprime, semiprime_cache,
               (max_precomp + 1) * sizeof(uint8_t));
        free(semiprime_cache);
    }
    semiprime_cache = new_semiprime;

    /* Allocate new SPF cache */
    idx_t *new_spf = (idx_t *)malloc((new_max + 1) * sizeof(idx_t));
    if (!new_spf)
        abort();
    if (spf_cache) {
        memcpy(new_spf, spf_cache,
               (max_precomp + 1) * sizeof(idx_t));
        free(spf_cache);
    }
    spf_cache = new_spf;

    /* Initialise SPF for the new entries */
    for (idx_t i = max_precomp + 1; i <= new_max; ++i)
        spf_cache[i] = i;   /* assume prime until proven otherwise */

    /* Sieve SPF up to the new maximum */
    idx_t limit = (idx_t)(sqrt((double)new_max));
    for (idx_t p = 2; p <= limit; ++p) {
        if (spf_cache[p] == p) {                /* p is prime */
            for (idx_t m = p * p; m <= new_max; m += p) {
                if (spf_cache[m] == m)
                    spf_cache[m] = p;
            }
        }
    }

    /* Flag semiprimes in the newly added interval */
    for (idx_t i = max_precomp + 1; i <= new_max; ++i) {
        if (i < 4) {                /* 1,2,3 are not semiprime */
            semiprime_cache[i] = 0;
            continue;
        }
        idx_t x = i;
        int cnt = 0;
        while (x > 1 && cnt <= 2) {
            ++cnt;
            idx_t f = spf_cache[x];
            x /= f;
        }
        semiprime_cache[i] = (cnt == 2) ? 1 : 0;
    }

    max_precomp = new_max;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) // guard against unsigned wrap‑around
        return 0;
    ensure_precomp(n);
    seq_t result = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i)
        if (semiprime_cache[i] && semiprime_cache[n - i])
            ++result;
    return result;
}
