/*
 * A138768  For a positive integer n, write the integers 1,2,...,n in the
 *          following order: first write 1 (round 0), then all primes less
 *          than or equal to n in increasing order (round 1), then 2p for all
 *          primes p with 2p<=n, also in increasing order (round 2), then 3p,
 *          then 4p and so on. Each number is writte...
 * https://oeis.org/A138768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A138768:  
   a(n) = the largest m (1 ≤ m ≤ n) for which m/largest_prime_factor(m) 
   is maximised; ties broken by larger m.  */
/* Uses the build_spf helper to obtain the smallest prime factor array. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static idx_t *spf_cache = NULL;
static idx_t spf_limit = 0;

/* Allocate or enlarge the SPF cache to at least "limit" */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf_cache) free(spf_cache);
    build_spf(limit, &spf_cache);
    spf_limit = limit;
}

seq_t a(idx_t n)
{
    /* The definition is only valid for n ≥ 1. */
    if (n < 1) return (seq_t)1;

    /* Have SPF table up to n. */
    ensure_spf(n);

    idx_t best_m   = 1;
    idx_t best_val = 0;   /* for m/lpf(m) */

    for (idx_t m = 2; m <= n; ++m) {
        idx_t temp = m;
        idx_t lpf  = 0;   /* largest prime factor encountered */

        /* Find the largest prime factor using the SPF array. */
        while (temp > 1) {
            idx_t p = spf_cache[temp];
            lpf = p;            /* p is current prime factor, may get updated */
            while (temp % p == 0) temp /= p;
        }

        /* m is divisible by lpf. */
        idx_t val = m / lpf;
        if (val > best_val || (val == best_val && m > best_m)) {
            best_val = val;
            best_m   = m;
        }
    }

    return (seq_t)best_m;
}
