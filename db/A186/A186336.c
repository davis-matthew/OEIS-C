/*
 * A186336  Number of ways of representing n as the sum of one or more
 *          consecutive semiprimes.
 * https://oeis.org/A186336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways of writing n as the sum of one or more *
 *            consecutive semiprimes (semiprimes = numbers which are the
 *            product of two primes, * counted with multiplicity). The
 *            implementation builds a sieve of the * smallest prime factor up
 *            to the current maximum requested n, extracts the * sem...
 * Range:     u8 n <= 825, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"   /* build_spf(limit, &spf) */
#include "../utils/is_prime.h"    /* is_prime(x) */

/* Static state shared by all calls */
static idx_t      *spf          = NULL;   /* smallest prime factor array, size = max_n_sieved+1 */
static idx_t      max_n_sieved  = 0;      /* maximum n for which spf is valid */
static idx_t      *semiprimes   = NULL;   /* array of semiprimes, up to max_n_sieved */
static size_t     semip_count   = 0;      /* number of semiprimes stored */
static size_t     semip_cap     = 0;      /* allocated capacity of semiprimes */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n < 4 cannot be expressed as a sum of one or more semiprimes */
    if (n < 4) return 0;

    /* Rebuild state if we have not sieved up to n yet */
    if (n > max_n_sieved) {
        /* Free any previous SPF array */
        free(spf);
        spf = NULL;

        /* Build SPF up to n (array size n+1). */
        build_spf(n, &spf);

        /* Resize semiprime storage if necessary */
        size_t new_cap = (size_t)n + 1;
        if (semip_cap < new_cap) {
            idx_t *tmp = (idx_t *)realloc(semiprimes, new_cap * sizeof(idx_t));
            if (!tmp) return 0;   /* allocation failed – return 0 as safe fallback */
            semiprimes = tmp;
            semip_cap  = new_cap;
        }

        /* Extract semiprimes ≤ n */
        semip_count = 0;
        for (idx_t x = 2; x <= n; ++x) {
            idx_t p = spf[x];
            if (p == 0) continue;            /* skip 1 */
            idx_t m = x / p;
            if (is_prime(m) && is_prime(p))  /* p and m are primes */
                semiprimes[semip_count++] = x;
        }

        max_n_sieved = n;
    }

    /* Count representations of n as a sum of consecutive semiprimes.
     * We use a classic two‑pointer sliding window over the sorted list of
     * semiprimes.  Complexity is O(k) where k = semip_count.               */
    seq_t count = 0;
    size_t i = 0, j = 0;
    seq_t sum  = 0;
    size_t sz = semip_count;

    while (i < sz) {
        while (j < sz && sum < (seq_t)n)
            sum += (seq_t)semiprimes[j++];
        if (sum == (seq_t)n)
            ++count;
        sum -= (seq_t)semiprimes[i++];
    }

    return count;
}
