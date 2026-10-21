/*
 * A210047  Number of (n+1)X4 0..2 arrays containing all values 0..2 with
 *          every 2X2 subblock having two distinct values, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A210047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for A210047 For n>14, a(n) = 24*a(n-1) -
 *            89*a(n-2) - 977*a(n-3) + 3094*a(n-4) +13063*a(n-5) -28268*a(n-6)
 *            -71414*a(n-7) +90536*a(n-8) +163472*a(n-9) -114928*a(n-10)
 *            -148256*a(n-11) +54080*a(n-12) +35328*a(n-13) -9216*a(n-14)
 *            Initial values: a(1)=75, a(2)=1271, a(3)=20871, a(4...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t known[15] = {
    0ULL,                        // unused 0 index
    75ULL,
    1271ULL,
    20871ULL,
    332891ULL,
    5169813ULL,
    78792305ULL,
    1184416509ULL,
    17624469579ULL,
    260264902127ULL,
    3821147552379ULL,
    55850881366677ULL,
    813499248661201ULL,
    11816892350999277ULL,
    171285288093188555ULL
};

static const int coeff[14] = {
    24,     -89,    -977,   3094,   13063,
   -28268, -71414, 90536, 163472, -114928,
   -148256, 54080, 35328, -9216
};

static seq_t *cache = NULL;      // 1‑based indexing
static idx_t cache_cap = 0;      // capacity (elements)
static idx_t cache_max = 0;      // highest term computed

/* Ensure cache can hold elements up to 'needed' (inclusive). */
static void ensure_capacity(idx_t needed) {
    if (needed < cache_cap)
        return;
    idx_t new_cap = needed * 2 + 128;  /* allow growth */
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* In a real implementation you might handle OOM. */
        abort();
    }
    cache = new_buf;
    cache_cap = new_cap;
}

/* Compute a(n) using memoization and the empirical recurrence. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined, but return 0 for safety */

    /* initialize known values on first use */
    if (cache_max == 0) {
        ensure_capacity(14 + 1);
        for (idx_t i = 1; i <= 14; ++i)
            cache[i] = known[i];
        cache_max = 14;
    }

    if (n <= cache_max)
        return cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = known[i];
            continue;
        }

        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 14; ++k) {
            seq_t term = cache[i - 1 - k];
            int c = coeff[k];
            if (c >= 0)
                sum_pos += (seq_t)c * term;
            else
                sum_neg += (seq_t)(-c) * term;
        }
        cache[i] = sum_pos - sum_neg;  /* assume non‑negative */
    }

    cache_max = n;
    return cache[n];
}
