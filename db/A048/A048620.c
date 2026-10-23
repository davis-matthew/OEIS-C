/*
 * A048620  a(n) is the maximal value of Omega(binomial(n,k)) over k, where
 *          Omega = A001222.
 * https://oeis.org/A048620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{0<=k<=n} Omega(binomial(n,k)) Computed by using
 *            Omega(n!) = sum_{i=2..n} Omega(i) and Omega(binomial(n,k)) =
 *            Omega(n!) - Omega(k!) - Omega((n-k)!). Here Omega(i) is the
 *            number of prime factors of i (with multiplicity)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  static data for memoisation  */
static idx_t   precomp_n = 0;                    /* >= 1, upto which prefix sums are computed   */
static uint32_t *spf      = NULL;                /* smallest prime factor for each i             */
static uint8_t  *omegaA   = NULL;                /* Omega(i) for each i, stored as 8‑bit int      */
static seq_t   *pref     = NULL;                /* pref[i] = sum_{t=2..i} Omega(t)              */

/*  Computes Omega(i) for all 0<=i<=n, and prefix sums pref[].  */
static void compute_prefix_up_to(idx_t n)
{
    size_t i, j;
    idx_t  N = n;

    /* Reallocate arrays to hold up to n */
    spf     = (uint32_t *)realloc(spf,     (N + 1) * sizeof(uint32_t));
    omegaA  = (uint8_t  *)realloc(omegaA,  (N + 1) * sizeof(uint8_t));
    pref    = (seq_t   *)realloc(pref,     (N + 1) * sizeof(seq_t));

    if (!spf || !omegaA || !pref) { /* allocation failed */
        abort();
    }

    /*  Initialize spf for the new range.  */
    for (i = 2; i <= (size_t)N; ++i) {
        spf[i] = 0;
    }
    for (i = 2; i <= (size_t)N; ++i) {
        if (spf[i] == 0) {                              /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= (uint64_t)N) {
                for (j = (size_t)i * i; j <= (size_t)N; j += (size_t)i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /*  Compute Omega(i) and prefix sums pref[i] = Omega(i!)  */
    omegaA[0] = 0;
    pref[0]   = 0;
    if (N >= 1) {
        omegaA[1] = 0;
        pref[1]   = 0;
    }
    for (i = 2; i <= (size_t)N; ++i) {
        uint32_t p   = spf[i];
        uint32_t rest = i / p;
        uint8_t o   = omegaA[rest] + 1;
        omegaA[i] = o;
        pref[i]   = pref[i - 1] + (seq_t)o;
    }

    precomp_n = N;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;              /* a(1) = 0 */

    /*  Ensure prefix sums are available up to n  */
    if (precomp_n < n) {
        compute_prefix_up_to(n);
    }

    seq_t best = 0;
    for (idx_t k = 1; k < n; ++k) {    /* k=0 and k=n give value 0 */
        seq_t val = pref[n] - pref[k] - pref[n - k];
        if (val > best) best = val;
    }

    return best;
}
