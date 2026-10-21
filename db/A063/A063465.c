/*
 * A063465  Number k such that omega(k) = omega(k+3), where omega(k) is the
 *          number of distinct prime divisors of k.
 * https://oeis.org/A063465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the terms of OEIS A063465.
   The sequence is defined as the integers k such that
   ω(k) = ω(k+3), where ω(m) is the number of distinct prime
   divisors of m.  The function `a(n)` returns the n-th term
   (a(1) = 2).  The algorithm maintains a cached list of
   found terms and an array of smallest prime factors (spf).
   When a request for a larger index is made, it extends the
   spf array and scans new k‑values until enough terms are
   collected.  No hard‑coded table is used.  The cache and
   spf array are expanded only as needed, making the runtime
   essentially linear in the maximum k examined.  All
   arithmetic on indices is performed with the native
   integer type `idx_t` (uint64_t).  The function can be
   called in any order. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Cache of sequence terms (k values) */
static idx_t *k_cache = NULL;   /* dynamic array of cached terms   */
static size_t k_len   = 0;       /* number of cached terms          */
static size_t k_cap   = 0;       /* allocated capacity of k_cache    */

/* Smallest prime factor array */
static idx_t *spf         = NULL;   /* spf[0 .. spf_limit]            */
static idx_t  spf_limit   = 0;     /* highest index for which spf is valid */

/* Largest k processed so far */
static idx_t last_k = 1;

/* Ensure spf array contains smallest prime factors up to at least 'limit'. */
static void ensure_spf(idx_t limit) {
    if (spf_limit >= limit) return;

    /* free old spf array if it exists */
    if (spf) free(spf);

    build_spf(limit, (idx_t **)&spf);   /* builds spf[0 .. limit] */
    spf_limit = limit;
}

/* Number of distinct prime factors ω(n) using the precomputed spf array. */
static idx_t omega(idx_t n) {
    idx_t cnt  = 0;
    idx_t prev = 0;
    while (n > 1) {
        idx_t p = spf[n];
        if (p == 0) p = n;          /* prime larger than sqrt(limit) */
        if (p != prev) {
            ++cnt;
            prev = p;
        }
        while (n % p == 0) n /= p;
    }
    return cnt;
}

/* Append a new term to the cache, expanding capacity if necessary. */
static void push_term(idx_t k) {
    if (k_len == k_cap) {
        size_t new_cap = k_cap ? (k_cap << 1) : 16;
        idx_t *new_arr = (idx_t *)realloc(k_cache, new_cap * sizeof(idx_t));
        if (!new_arr) abort();
        k_cache = new_arr;
        k_cap   = new_cap;
    }
    k_cache[k_len++] = k;
}

/* Build terms until the cache has at least 'n_need' items. */
static void ensure_terms(idx_t n_need) {
    while (k_len < n_need) {
        /* Expand search range.  Roughly double the current range each time. */
        idx_t next_limit = last_k * 2 + 200;
        if (next_limit <= last_k) next_limit = last_k + 200;   /* safety */

        /* Ensure spf covers the range needed for checks involving k+3. */
        ensure_spf(next_limit + 3);

        for (idx_t k = last_k + 1; k <= next_limit; ++k) {
            if (omega(k) == omega(k + 3)) {
                push_term(k);
            }
        }
        last_k = next_limit;
    }
}

/* External function requested by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of bounds (not defined) */
    ensure_terms(n);
    return (seq_t)k_cache[n - 1];
}
