/*
 * A130065  a(n) = (n / GreatestPrimeFactor(n)) * SmallestPrimeFactor(n).
 * https://oeis.org/A130065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / gpf(n)) * spf(n) where spf(n) is the smallest prime
 *            factor of n and gpf(n) is the greatest prime factor of n. This
 *            implementation precomputes the smallest and greatest prime
 *            factors up to the maximum n requested in a static cache; it
 *            works for any seq_t/idx_t type combination supporte...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static cache for smallest and greatest prime factors  */
static uint64_t *spf   = NULL;   /* spf[i] – smallest prime factor of i  */
static uint64_t *gpf   = NULL;   /* gpf[i] – greatest prime factor of i  */
static idx_t    max_n = 0;       /* largest n that has been precomputed     */

/* Ensure spf[1..n] and gpf[1..n] are computed. */
static void
ensure_arrays(idx_t n)
{
    if (n <= max_n) return;            /* already available */

    /* Allocate new arrays large enough for 0..n indices */
    uint64_t *new_spf = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    uint64_t *new_gpf = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!new_spf || !new_gpf) abort();

    /* Copy existing data if any and free old arrays */
    if (spf) {
        memcpy(new_spf, spf, (max_n + 1) * sizeof(uint64_t));
        free(spf);
    }
    if (gpf) {
        memcpy(new_gpf, gpf, (max_n + 1) * sizeof(uint64_t));
        free(gpf);
    }

    /* Initialise new entries to zero */
    if (n > max_n) {
        memset(new_spf + (max_n + 1), 0,
               (n - max_n) * sizeof(uint64_t));
        memset(new_gpf + (max_n + 1), 0,
               (n - max_n) * sizeof(uint64_t));
    }

    /* Compute smallest prime factors up to n */
    for (idx_t i = 2; i <= n; ++i) new_spf[i] = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (new_spf[p] == 0) {          /* p is prime */
            new_spf[p] = p;
            for (idx_t m = p * 2ULL; m <= n; m += p)
                if (new_spf[m] == 0) new_spf[m] = p;
        }
    }
    new_spf[1] = 1;                      /* convention for 1 */

    /* Compute greatest prime factors using spf */
    new_gpf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t x = i;
        uint64_t maxp = 1;
        while (x > 1) {
            uint64_t p = new_spf[x];
            if (p > maxp) maxp = p;
            do { x /= p; } while (x % p == 0);
        }
        new_gpf[i] = maxp;
    }

    spf = new_spf;
    gpf = new_gpf;
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n = 0 */

    ensure_arrays(n);

    /* (n / gpf[n]) * spf[n] */
    idx_t part = n / gpf[n];
    seq_t result = (seq_t)part * (seq_t)spf[n];
    return result;
}
