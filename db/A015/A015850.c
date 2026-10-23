/*
 * A015850  Numbers k such that phi(k + 12) | sigma(k) for k not congruent to
 *          0 (mod 3).
 * https://oeis.org/A015850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   append_candidate, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015850: numbers k such that phi(k+12) divides sigma(k)
   and k is not divisible by 3.  The function enumerates these
   numbers in increasing order and returns the n-th term (a(1) is
   the first listed value).  Implementation uses an SPF sieve followed
   by simple factorisation to compute phi and sigma on the fly.  */

/* ---- required includes -------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_spf.h"        /* build_spf(limit, &spf)          */
#include "../utils/append_candidate.h" /* append_candidate(&arr,&len,&cap,val) */

/* ---- static data -------------------------------------------------------- */
static seq_t *g_seq     = NULL;  /* collected terms */
static size_t g_len     = 0;     /* number of terms collected */
static size_t g_cap     = 0;     /* allocated capacity */
static idx_t *g_spf     = NULL;  /* smallest prime factor array */
static idx_t  g_spf_limit = 0;   /* size of spf array (max index) */
static idx_t  g_next_k  = 1;     /* next k to examine */

/* ---- local helpers ----------------------------------------------------- */
static seq_t _sigma(idx_t n, const idx_t *spf)
{
    uint64_t x = n;
    seq_t   res = 1;
    while (x > 1) {
        uint64_t p = spf[x];
        if (p == 0) p = x;              /* safety for primes */
        uint64_t sum  = 1;
        uint64_t term = 1;
        while (x % p == 0) {
            x   /= p;
            term *= p;
            sum  += term;
        }
        res *= sum;
    }
    return res;
}

static seq_t _phi(idx_t n, const idx_t *spf)
{
    uint64_t x = n;
    seq_t   res = (seq_t)n;
    while (x > 1) {
        uint64_t p = spf[x];
        if (p == 0) p = x;              /* safety for primes */
        res -= res / p;
        while (x % p == 0) x /= p;
    }
    return res;
}

/* ---- main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but keep safe */

    /* initialise the SPF list once, using a generous upper bound   */
    if (g_spf == NULL) {
        idx_t limit = 10000000;          /* 10 000 000 > enough for n≤10000 */
        build_spf(limit, &g_spf);
        g_spf_limit = limit;
    }

    /* collect terms until we have the requested index */
    while (g_len < n) {
        /* if the precomputed spf does not reach k+12, enlarge it */
        if (g_next_k + 12 > g_spf_limit) {
            /* for the chosen limit this should never happen */
            break;
        }
        if (g_next_k % 3 == 0) {         /* skip multiples of 3 */
            ++g_next_k;
            continue;
        }

        seq_t sigma_k  = _sigma(g_next_k, g_spf);
        seq_t phi_k12  = _phi(g_next_k + 12, g_spf);
        if (sigma_k % phi_k12 == 0) {
            append_candidate(&g_seq, &g_len, &g_cap, (seq_t)g_next_k);
        }
        ++g_next_k;
    }

    return g_seq[n - 1];
}
