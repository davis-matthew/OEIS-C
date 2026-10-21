/*
 * A052604  E.g.f. (1-x)/(1-2x-x^3+x^4).
 * https://oeis.org/A052604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052604
   E.g.f.: (1‑x)/(1‑2x‑x^3+x^4)
   Recurrence (for n ≥ 0):
   (n^4+10n^3+35n^2+50n+24)·a(n)
   + (−n^3−9n^2−26n−24)·a(n+1)
   + (−2n−8)·a(n+3)
   + a(n+4) = 0
   with a(0)=1, a(1)=1, a(2)=4, a(3)=30. */

/* --- implementation --------------------------------------------- */

#include <stdlib.h>
#include <limits.h>

/* static cache for memoisation */
static seq_t *cache     = NULL;   /* array of computed terms     */
static size_t   cache_cap  = 0;   /* allocated capacity          */
static idx_t    cache_filled = 0; /* number of terms already computed (highest index+1) */

/* ensure that cache has room for at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) { abort(); }           /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;

    /* initialise the first four values if not already done */
    if (cache_filled == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)30;
        cache_filled = 4;            /* indices 0..3 are now available */
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* trivial small indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)30;

    /* make sure cache can hold the requested index */
    ensure_capacity(n + 1);

    /* compute terms that are still missing, sequentially */
    for (idx_t i = cache_filled; i <= n; ++i) {
        /* use the recurrence:
           a(i) = -A*a(i-4) + B*a(i-3) + C*a(i-1)
           where
           A = k^4 + 10k^3 + 35k^2 + 50k + 24
           B = k^3 + 9k^2 + 26k + 24
           C = 2k + 8
           with k = i-4
        */
        idx_t k      = i - 4;
        seq_t kseq   = (seq_t)k;
        seq_t k2     = kseq * kseq;
        seq_t k3     = k2 * kseq;
        seq_t k4     = k3 * kseq;

        seq_t A = k4 + (seq_t)10 * k3 + (seq_t)35 * k2 + (seq_t)50 * kseq + (seq_t)24;
        seq_t B = k3 + (seq_t)9 * k2 + (seq_t)26 * kseq + (seq_t)24;
        seq_t C = (seq_t)2 * kseq + (seq_t)8;

        seq_t term1 = B * cache[k + 1];
        seq_t term2 = C * cache[k + 3];
        seq_t term3 = A * cache[k];

        /* subtraction is safe because the recurrence guarantees the
           result is non‑negative */
        cache[i] = term1 + term2 - term3;
    }

    cache_filled = n + 1;      /* all indices up to n are now available */
    return cache[n];
}
