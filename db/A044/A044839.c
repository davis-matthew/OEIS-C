/*
 * A044839  Positive integers having more base-13 runs of even length than
 *          odd.
 * https://oeis.org/A044839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) is the nth positive integer whose base‑13
 * representation has more runs (maximal consecutive equal digits)
 * of even length than of odd length.  The sequence starts
 *
 *    14, 28, 42, 56, 70, 84, 98, 112, 126, 140, 154, 168, …
 *
 * The routine keeps a static cache of already found values so that
 * arbitrary‑order calls are efficient.  Each candidate is tested by
 * examining its base‑13 digits from least significant to most
 * significant and counting run lengths on the fly.  No additional
 * external utilities are required; only the standard library is used. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* how many elements are stored */
static size_t cache_cap = 0;   /* allocated capacity of cache */
static seq_t next_num = 1;     /* next candidate to test */

/* count whether a number has more even‑length runs than odd‑length runs */
static bool more_even_runs(seq_t x)
{
    /* Handle 0 explicitly; its representation is just a single digit 0 */
    if (x == 0) {
        return false;  /* 0 has one odd run, hence not “more even” */
    }

    seq_t v = x;
    /* first (least significant) digit */
    uint8_t prev = (uint8_t)(v % 13);
    v /= 13;
    size_t run_len = 1;
    size_t even_runs = 0, odd_runs = 0;

    /* process remaining digits */
    while (v > 0) {
        uint8_t cur = (uint8_t)(v % 13);
        v /= 13;
        if (cur == prev) {
            ++run_len;
        } else {
            if ((run_len & 1) == 0) {
                ++even_runs;
            } else {
                ++odd_runs;
            }
            prev = cur;
            run_len = 1;
        }
    }

    /* account for the final run */
    if ((run_len & 1) == 0) {
        ++even_runs;
    } else {
        ++odd_runs;
    }

    return even_runs > odd_runs;
}

/* enlarge cache storage if we need one more element */
static void ensure_cache_capacity(void)
{
    if (cache_cap >= cache_len + 1) {
        return;
    }
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    if (new_cap < cache_len + 1) {
        new_cap = cache_len + 1;
    }
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (new_cache == NULL) {
        /* In a real program you would handle allocation failure;
           here we simply abort to keep the implementation brief. */
        abort();
    }
    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* external linkage: the OEIS sequence accessor */
seq_t a(idx_t n)
{
    /* No defined value for n == 0; returning 0 is harmless */
    if (n == 0) {
        return 0;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (more_even_runs(next_num)) {
            ensure_cache_capacity();
            cache[cache_len++] = next_num;
        }
        next_num = next_num + 1;
    }

    return cache[n - 1];
}
