/*
 * A022954  a(n) = a(n-1) + c(n+1) for n >= 3, a( ) increasing, given a(1)=1,
 *          a(2)=8; where c( ) is complement of a( ).
 * https://oeis.org/A022954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022954:
 *  a(n) = a(n-1) + c(n+1)   for n ≥ 3,
 *  a(1) = 1, a(2) = 8;
 *  c(k) is the k‑th positive integer not in the sequence.
 *  This implementation builds the sequence incrementally, keeping an
 *  array of previously computed terms and scanning successive integers
 *  to find the required complement numbers.  It works for all builds
 *  where seq_t and idx_t are integer types; for arbitrary‑precision
 *  seq_t the comparisons of seq_t values are handled by the language's
 *  operator overloading (C++17) or by built‑in integer operators (C
 * 11). */

#include <stdlib.h>
#include <stdint.h>

/* Ensure that *arr has capacity >= needed. Grow by doubling if
   necessary.  The array holds seq_t values. */
static void ensure_cache_cap(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap == 0) ? 2 : (*cap << 1);
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    *arr = tmp;
    *cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static state shared across calls. */
    static int initialized = 0;
    static seq_t *cache = NULL;       /* array of computed terms */
    static size_t cache_cap = 0;      /* current capacity of cache */
    static size_t cache_len = 0;      /* number of terms stored */

    /* Variables used for finding complement numbers. */
    static seq_t last_candidate = 1;  /* next integer to test as a candidate */
    static idx_t seq_ptr = 0;         /* index into cache for comparison */
    static idx_t missing_seen = 0;    /* count of complement numbers encountered */

    /* Initialization on first call. */
    if (!initialized) {
        initialized = 1;

        /* Allocate space for the first two terms. */
        ensure_cache_cap(2, &cache, &cache_cap);
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)8;          /* a(2) */
        cache_len = 2;

        /* Set search state just after the two known terms. */
        last_candidate = 1;           /* start at 1, will be advanced in loop */
        seq_ptr = 0;                  /* will be advanced to point past 1 */
        missing_seen = 0;
    }

    /* Handle trivial indexed access. */
    if (n == 1) return cache[0];
    if (n == 2) return cache[1];

    /* If the requested index is already computed, return it. */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow cache to fit the requested index (safety). */
    ensure_cache_cap((size_t)n, &cache, &cache_cap);

    /* Compute terms up to the requested index. */
    while (cache_len < (size_t)n) {
        /* We are computing a(k) where k = cache_len + 1. */
        idx_t k = (idx_t)cache_len + 1;

        /* Need the (k+1)-th missing number: c(k+1). */
        idx_t target_missing = k + 1;    /* value of missing_seen to reach */

        seq_t c_val = 0;

        /* Scan successive integers to find the required complement. */
        while (missing_seen < target_missing) {
            /* Advance seq_ptr to the first cache element >= last_candidate. */
            while (seq_ptr < (idx_t)cache_len && cache[seq_ptr] < last_candidate) {
                ++seq_ptr;
            }

            if (seq_ptr < (idx_t)cache_len && cache[seq_ptr] == last_candidate) {
                /* Candidate is in the sequence; skip it. */
                ++last_candidate;
            } else {
                /* candidate is a complement number. */
                ++missing_seen;
                if (missing_seen == target_missing) {
                    c_val = last_candidate;
                }
                ++last_candidate;
            }
        }

        /* Compute a(k) = a(k-1) + c(k+1). */
        seq_t prev = cache[cache_len - 1];   /* a(k-1) */
        seq_t ak = prev + c_val;             /* a(k) */

        /* Append new term to cache. */
        cache[cache_len] = ak;
        ++cache_len;
    }

    return cache[n - 1];
}
