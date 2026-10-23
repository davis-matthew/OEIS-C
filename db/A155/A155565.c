/*
 * A155565  Intersection of A001481 and A020669: N = a^2 + b^2 = c^2 + 5d^2
 *          for some integers a,b,c,d.
 * https://oeis.org/A155565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer N such that N = a^2 + b^2 = c^2 + 5 d^2
 *            for some integers a,b,c,d (intersection of A001481 and A020669)
 *            a(1) = 0, a(2) = 1, a(3) = 4, ... (as in the OEIS entry)
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------- */
/* Helper to compute floor( sqrt(n) ) without using floating point */
/* ------------------------------------------------------------- */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, res = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* ------------------------------------------------------------- */
/* Global static state for memoising the sequence               */
/* ------------------------------------------------------------- */
static seq_t *term_cache  = NULL;    /* cached sequence values   */
static size_t cache_cap   = 0;        /* allocated size          */
static size_t cache_len   = 0;        /* number of cached terms  */

static bool *rep_two_sq   = NULL;    /* table for numbers a^2+b^2 */
static bool *rep_x2p5_sq  = NULL;    /* table for c^2+5d^2      */
static idx_t rep_limit    = 0;        /* current capacity of tables */

static idx_t next_test    = 0;        /* next integer to test    */

/* ------------------------------------------------------------- */
/* Build a representation table up to LIMIT (inclusive).         */
/* If is_plus5 is true, build for c^2 + 5 d^2, otherwise for */ 
/* a^2 + b^2.                                                   */
/* ------------------------------------------------------------- */
static void build_rep_table(idx_t limit, bool *arr, bool is_plus5)
{
    if (!arr) return;

    idx_t sq_limit = isqrt_idx(limit);
    for (idx_t i = 0; i <= sq_limit; ++i) {
        idx_t i2 = i * i;
        if (is_plus5) {
            /* c^2 + 5 d^2 */
            /* find max d: 5*d^2 <= limit - i^2          */
            idx_t rem = limit - i2;
            if (rem < 5) continue;
            idx_t d_max = isqrt_idx(rem / 5);
            for (idx_t d = 0; d <= d_max; ++d) {
                idx_t val = i2 + 5 * d * d;
                if (val <= limit) arr[val] = true;
            }
        } else {
            /* a^2 + b^2 */
            idx_t rem = limit - i2;
            if (rem < 0) continue;
            idx_t b_max = isqrt_idx(rem);
            for (idx_t b = 0; b <= b_max; ++b) {
                idx_t val = i2 + b * b;
                if (val <= limit) arr[val] = true;
            }
        }
    }
}

/* ------------------------------------------------------------- */
/* Grow the representation tables to at least NEW_LIMIT (inclusive). */
/* Re‑allocate and rebuild the tables from scratch.              */
/* ------------------------------------------------------------- */
static void ensure_rep_limit(idx_t new_limit)
{
    if (new_limit <= rep_limit) return; /* already large enough */

    /* free old tables */
    free(rep_two_sq);
    free(rep_x2p5_sq);

    /* allocate new tables */
    rep_two_sq = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));
    rep_x2p5_sq = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));

    /* build representations */
    build_rep_table(new_limit, rep_two_sq, false);   /* a^2 + b^2   */
    build_rep_table(new_limit, rep_x2p5_sq, true);   /* c^2 + 5d^2 */

    rep_limit = new_limit;
}

/* ------------------------------------------------------------- */
/* Grow the cached sequence array.                               */
/* ------------------------------------------------------------- */
static void grow_term_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
    term_cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------- */
/* Scan through integers from NEXT_TEST up to LIM inclusive,     */
/* adding any that satisfy both representations to the cache.   */
/* ------------------------------------------------------------- */
static void add_terms_up_to(idx_t lim)
{
    if (next_test > lim) return;

    for (idx_t x = next_test; x <= lim; ++x) {
        if (rep_two_sq[x] && rep_x2p5_sq[x]) {
            if (cache_len == cache_cap) grow_term_cache();
            term_cache[cache_len++] = (seq_t)x;
        }
    }
    next_test = lim + 1;
}

/* ------------------------------------------------------------- */
/* Public function implementing the OEIS sequence               */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached.  If not, grow limits. */
    while (cache_len < n) {
        /* Choose a new sieve/representation limit.  Start small, then double. */
        idx_t new_lim = rep_limit ? rep_limit << 1 : 1000;
        if (new_lim < next_test) new_lim = next_test;
        ensure_rep_limit(new_lim);
        add_terms_up_to(new_lim);
    }
    return term_cache[n - 1];
}
