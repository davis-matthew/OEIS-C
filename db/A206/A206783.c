/*
 * A206783  Number of nX6 0..1 arrays avoiding 0 0 1 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A206783.
   Number of n×6 binary arrays avoiding horizontal 001 and vertical 011.
   The function a(idx_t n) returns a(n) for n≥1.  It uses dynamic
   programming with memoisation so that it can answer calls in any order.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Returns the n-th term of A206783 (n ≥ 1).  */
seq_t a(idx_t n)
{
    /* --- static data --------------------------------------------------- */
    static bool initialized = false;
    static idx_t computed_n = 0;       /* largest n with cached value   */
    static seq_t *cache        = NULL; /* cache[1..computed_n]          */

    /* The DP data is shared for all calls.  */
    static size_t L               = 0;     /* number of legal row masks   */
    static unsigned char *leg     = NULL;    /* legal row masks (0..63)   */

    static size_t S               = 0;     /* number of states (prevprev, prev) */
    static seq_t *dp_cur          = NULL;   /* DP for current n   */
    static seq_t *dp_next         = NULL;   /* DP for next n      */
    static size_t *next_start     = NULL;   /* start index of list of next states for each state */
    static unsigned char *next_cnt  = NULL;    /* number of next states per state   */
    static size_t *next_indices   = NULL;    /* flat list of next state indices   */

    /* --- initialization ----------------------------------------------- */
    if (!initialized) {
        /* 1. legal row masks (avoid horizontal 001) */
        unsigned char masks[64];
        size_t i;
        for (i = 0; i < 64; ++i) {
            int bad = 0;
            for (int pos = 0; pos <= 3 && !bad; ++pos) {
                if (((i >> pos) & 7u) == 1u) /* 0 0 1 pattern */
                    bad = 1;
            }
            masks[(size_t)(i)] = (unsigned char)(bad? 255 : i);
        }

        /* Count and store legal masks */
        size_t count = 0;
        for (i = 0; i < 64; ++i)
            if (masks[i] != 255u)
                ++count;
        L = count;
        leg = (unsigned char *)malloc(L * sizeof(unsigned char));
        if (!leg) exit(EXIT_FAILURE);
        size_t idx = 0;
        for (i = 0; i < 64; ++i)
            if (masks[i] != 255u)
                leg[idx++] = (unsigned char)i;  /* legal mask */

        /* 2. States: pair (prevprev, prev) */
        S = L * L;

        /* 3. Transition lists */
        next_start = (size_t *)malloc(S * sizeof(size_t));
        next_cnt   = (unsigned char *)malloc(S * sizeof(unsigned char));
        if (!next_start || !next_cnt) exit(EXIT_FAILURE);
        /* Rough upper bound: L*L*L transitions */
        next_indices = (size_t *)malloc(S * L * sizeof(size_t));
        if (!next_indices) exit(EXIT_FAILURE);

        for (size_t s = 0; s < S; ++s) {
            size_t start = 0;    /* will be overwritten */
            unsigned char cnt = 0;
            size_t i_prevprev = s / L;
            size_t i_prev     = s % L;
            unsigned char mask_pp = leg[i_prevprev];
            unsigned char mask_p  = leg[i_prev];

            start = 0; /* placeholder, will set after counting */
            /* Count valid next rows */
            for (size_t k = 0; k < L; ++k) {
                unsigned char mask_n = leg[k];
                bool ok = true;
                for (int col = 0; col < 6 && ok; ++col) {
                    if (((mask_pp >> col) & 1u) == 0 &&
                        ((mask_p >> col)  & 1u) == 1u &&
                        ((mask_n >> col)  & 1u) == 1u)
                        ok = false;
                }
                if (ok)
                    ++cnt;
            }
            next_start[s] = 0; /* placeholder, will set later */
            next_cnt[s]   = cnt;
        }

        /* Fill the actual indices and compute real start positions */
        size_t pos = 0;
        for (size_t s = 0; s < S; ++s) {
            size_t i_prevprev = s / L;
            size_t i_prev     = s % L;
            unsigned char mask_pp = leg[i_prevprev];
            unsigned char mask_p  = leg[i_prev];

            next_start[s] = pos;
            for (size_t k = 0; k < L; ++k) {
                unsigned char mask_n = leg[k];
                bool ok = true;
                for (int col = 0; col < 6 && ok; ++col) {
                    if (((mask_pp >> col) & 1u) == 0 &&
                        ((mask_p >> col)  & 1u) == 1u &&
                        ((mask_n >> col)  & 1u) == 1u)
                        ok = false;
                }
                if (ok) {
                    next_indices[pos++] = i_prev * L + k; /* new state (prev, next) */
                }
            }
        }
        /* 4. DP arrays */
        dp_cur  = (seq_t *)calloc(S, sizeof(seq_t));
        dp_next = (seq_t *)calloc(S, sizeof(seq_t));
        if (!dp_cur || !dp_next) exit(EXIT_FAILURE);

        /* 5. Base cases */
        /* a(1): number of legal rows */
        cache = (seq_t *)malloc(3 * sizeof(seq_t)); /* index 0 unused */
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)L;  /* a(1) = L == 33 */
        /* a(2): any pair of legal rows, no vertical restriction yet */
        seq_t sum = 0;
        for (size_t s = 0; s < S; ++s) {
            dp_cur[s] = 1;
            sum += 1;
        }
        cache[2] = sum;  /* a(2) = L*L == 1089 */
        computed_n = 2;

        initialized = true;
    }

    /* --- extend cache if needed ---------------------------------------- */
    if (n > computed_n) {
        /* Reallocate cache */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        for (idx_t k = computed_n + 1; k <= n; ++k) {
            /* Clear dp_next */
            for (size_t s = 0; s < S; ++s)
                dp_next[s] = 0;

            /* Transition */
            for (size_t s = 0; s < S; ++s) {
                seq_t val = dp_cur[s];
                if (val == 0)
                    continue;
                size_t start = next_start[s];
                unsigned char cnt  = next_cnt[s];
                for (unsigned char t = 0; t < cnt; ++t) {
                    dp_next[next_indices[start + t]] += val;
                }
            }

            /* Sum to obtain a(k) */
            seq_t total = 0;
            for (size_t s = 0; s < S; ++s)
                total += dp_next[s];
            cache[k] = total;

            /* Swap dp_cur and dp_next for next iteration */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }
        computed_n = n;
    }

    return cache[n];
}
