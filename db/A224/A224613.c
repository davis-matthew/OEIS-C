/*
 * A224613  a(n) = sigma(6*n).
 * https://oeis.org/A224613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(6*n) (sum of divisors of 6*n)
 * Range:     u8 n <= 17, u16 n <= 2939, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   // provides build_spf()

/* Cache for the smallest‑prime‑factor table */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure that spf contains SPF values up to at least 'need' (inclusive) */
static void ensure_spf(idx_t need)
{
    if (need <= spf_limit) return;
    if (spf != NULL) free(spf);
    build_spf(need, &spf);
    spf_limit = need;
}

/* Compute sigma(m) for a 64‑bit integer using the SPF table.
   The result is returned as a seq_t (unsigned integer, possibly arbitrary size). */
static seq_t sigma_u64(uint64_t m)
{
    seq_t result = (seq_t)1;
    while (m > 1) {
        uint64_t p = (uint64_t)spf[m];      /* smallest prime divisor of m   */
        int exp = 0;
        while (m % p == 0) {                 /* count exponent of p in m     */
            m /= p;
            exp++;
        }
        /* Compute (p^(exp+1)-1)/(p-1)  as the sum 1+p+...+p^exp   */
        seq_t term = (seq_t)1;               /* p^0                     */
        seq_t sum  = (seq_t)1;               /* starts with 1            */
        for (int i = 0; i < exp; ++i) {
            term *= (seq_t)p;                /* term = p^(i+1)           */
            sum += term;                    /* accumulate sum            */
        }
        result *= sum;
    }
    return result;
}

/* Main OEIS function.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Compute 6*n as an idx_t to avoid overflow on very large n. */
    idx_t m = n * (idx_t)6ULL;

    /* Prepare SPF table up to m. */
    ensure_spf(m);

    /* Compute sigma(6*n).  m fits in uint64_t for all practical test values. */
    return sigma_u64((uint64_t)m);
}
