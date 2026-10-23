/*
 * A025387  Numbers that are the sum of 4 distinct nonzero squares in 3 or
 *          more ways.
 * https://oeis.org/A025387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025387: Numbers that are the sum of 4 distinct non‑zero squares
   in 3 or more distinct unordered ways.
   The function builds a sieve of distinct squares up to a dynamically
   increasing bound and extracts the required terms.  */

/* Standard headers (seq_t and idx_t are defined by the build system). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Integer square root for unsigned 64‑bit values.  */
static idx_t
isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low  = 0;
    idx_t high = (n >> 1) + 1;        /* high > sqrt(n) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == (__int128)n) return mid;
        if (sq < (__int128)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* --------------------------------------------------------------------- */
/* Persistent storage for the sequence values that have been generated. */
static seq_t *terms = NULL;      /* array of sequence terms      */
static size_t terms_len = 0;     /* how many terms are stored    */
static size_t terms_cap = 0;     /* current capacity              */

static idx_t current_limit = 0;  /* largest bound processed       */

/* --------------------------------------------------------------------- */
/* Ensure that 'terms' can hold at least 'need' elements. */
static void
grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Recompute the sieve and term list up to the given limit. */
static void
recompute(idx_t limit)
{
    /* Allocate dynamic programming tables: dp[t][s] = number of ways
       to obtain sum s using t distinct squares (capped at 3).          */
    size_t sz = (size_t)limit + 1;
    uint8_t *dp[5];
    for (int t = 0; t < 5; ++t) {
        dp[t] = (uint8_t *)calloc(sz, sizeof(uint8_t));
        if (!dp[t]) abort();
    }

    dp[0][0] = 1;   /* one way to choose zero squares that sum to zero      */

    idx_t maxk = isqrt_u64(limit);          /* largest square root to consider      */
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t v = k * k;                     /* current square value                 */
        for (int t = 4; t >= 1; --t) {
            for (size_t s = sz - 1; s >= (size_t)v; --s) {
                uint8_t add = dp[t-1][s - v];
                if (add && dp[t][s] < 3) {
                    uint8_t sum = dp[t][s] + add;
                    dp[t][s] = sum > 3 ? 3 : sum;
                }
            }
        }
    }

    /* Build the list of terms that satisfy the 3‑way condition. */
    grow_terms(1);
    terms_len = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        if (dp[4][(size_t)n] >= 3) {
            if (terms_len >= terms_cap) grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)n;
        }
    }

    /* Free DP tables. */
    for (int t = 0; t < 5; ++t) free(dp[t]);

    /* Update the global limit. */
    current_limit = limit;
}

/* --------------------------------------------------------------------- */
/* External linkage: returns the nth term of the sequence (1‑based).   */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* behaviour below offset is undefined */

    /* Generate terms until we have at least 'n' of them. */
    while (terms_len < (size_t)n) {
        /* Determine the next bound to use. */
        idx_t next_limit;
        if (current_limit == 0) {
            next_limit = 500;                      /* start small          */
        } else {
            next_limit = current_limit * 2;        /* double each time     */
        }
        recompute(next_limit);
    }

    return terms[n - 1];
}
