/*
 * A098520  E.g.f. exp(x)*BesselI(1,4*x)/2.
 * https://oeis.org/A098520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A098520
 *  a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k) * binomial(n-k,k+1) * 4^k
 *  Recurrence (for n ≥ 2):
 *    (n-1)*(n+1)*a(n) = n*(2n-1)*a(n-1) + 15*n*(n-1)*a(n-2)
 *  with a(0) = 0, a(1) = 1.
 *
 *  Implementation memoises values and applies the recurrence using
 *  128‑bit arithmetic when seq_t is 64‑bit, otherwise normal
 *  seq_t operations (which work for arbitrary‑precision builds).
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Cache holding computed terms 0 .. cache_last. */
static seq_t   *cache = NULL;  /* array of seq_t values           */
static idx_t    cache_len  = 0;/* allocated length of cache array */
static idx_t    cache_last = 1;/* highest index that has a value */

/* Ensure the cache can hold terms up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;                /* already large enough */

    idx_t new_len = cache_len ? cache_len : 4; /* start small (4 slots) */
    while (new_len <= n) new_len <<= 1;        /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                        /* allocation failure */
    cache = tmp;
    cache_len = new_len;

    /* Initialise base cases on first allocation. */
    if (new_len == 4) { /* first time we allocated space */
        cache[0] = 0;
        cache[1] = 1;
    }
}

/* Compute all terms up to and including index n.  Uses the stored
 * recurrence; assumes cache[0] and cache[1] are defined. */
static void compute_until(idx_t n)
{
    if (n <= cache_last) return;   /* nothing to do */

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (sizeof(seq_t) <= sizeof(unsigned long long)) { /* 64‑bit seq_t */
            unsigned __int128 ui = i;
            unsigned __int128 term1 = ui * (unsigned __int128)(2*i - 1) * (unsigned __int128)cache[i-1];
            unsigned __int128 term2 = (unsigned __int128)15 * ui * (unsigned __int128)(i-1) * (unsigned __int128)cache[i-2];
            unsigned __int128 num   = term1 + term2;
            unsigned __int128 den   = (ui - 1) * (ui + 1);
            cache[i] = (seq_t)(num / den);          /* exact division */
        } else { /* arbitrary‑precision seq_t */
            seq_t term1 = cache[i-1];
            term1 = term1 * (seq_t)i * (seq_t)(2*i - 1);

            seq_t term2 = cache[i-2];
            term2 = term2 * (seq_t)15 * (seq_t)i * (seq_t)(i-1);

            seq_t num = term1 + term2;
            seq_t den = (seq_t)(i - 1) * (seq_t)(i + 1);

            cache[i] = num / den;
        }
    }

    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* base case, works even if cache not allocated */
    ensure_cache(n);
    compute_until(n);
    return cache[n];
}
