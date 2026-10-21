/*
 * A097360  Number of partitions of n into parts not less than the smallest
 *          prime factor of n.
 * https://oeis.org/A097360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n using only parts not less than
 *            the smallest prime factor of n. The computation uses a
 *            dynamic‑programming scheme that iterates over possible minimum
 *            parts in decreasing order
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc, NULL */

/* The build supplies the integer types idx_t and seq_t.  They are used
   directly; no typedef is made here. */

/* ---------- static state ------------------------------------------------- */

/* Maximum n for which the tables are currently valid. */
static idx_t  max_cached = 0;
/* Cached answer for each n.  cache[n] == a(n). */
static seq_t *cache = NULL;

/* ---------- helper functions --------------------------------------------- */

/* Compute all values up to N (inclusive).  The result is stored in
   the global array 'cache', which is resized accordingly. */
static void compute_all(idx_t N)
{
    /* Allocate tables.  All sizes are N+1 to use 1‑based indexing. */
    size_t size = (size_t)N + 1;
    uint32_t *spf   = (uint32_t *)malloc(size * sizeof(uint32_t));
    uint32_t *head  = (uint32_t *)malloc(size * sizeof(uint32_t));
    uint32_t *next  = (uint32_t *)malloc(size * sizeof(uint32_t));
    seq_t    *dp_prev = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t    *dp_cur  = (seq_t *)malloc(size * sizeof(seq_t));

    /* Ensure tables are valid; on allocation failure abort. */
    if (!spf || !head || !next || !dp_prev || !dp_cur) exit(EXIT_FAILURE);

    /* Initialise arrays.  head and next store linked lists of n by spf. */
    for (size_t i = 0; i < size; ++i) {
        spf[i]  = 0;
        head[i] = 0;
        next[i] = 0;
    }

    /* ----- Linear sieve for smallest prime factor (spf). ---- */
    if (N >= 1) spf[1] = 1;
    uint32_t *pr = (uint32_t *)malloc(size * sizeof(uint32_t));
    size_t pc = 0;
    for (size_t i = 2; i <= (size_t)N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            pr[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = pr[j];
            size_t prod = i * (size_t)p;
            if (prod > (size_t)N) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
    free(pr);

    /* Build linked lists: for each n>=2, link it in list of its spf. */
    for (size_t n = 2; n <= (size_t)N; ++n) {
        uint32_t k = spf[n];
        next[n] = head[k];
        head[k] = (uint32_t)n;
    }

    /* Allocate / reallocate cache array. */
    if (!cache || max_cached < N) {
        seq_t *new_cache = (seq_t *)realloc(cache, size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
    }

    /* ---------- Dynamic programming over minimum part ---------- */
    /* dp_prev holds partitions with minimum part > current p.
       dp_cur will host partitions with minimum part >= current p. */
    dp_prev[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)N; ++i) dp_prev[i] = (seq_t)0;

    /* Process p from N down to 2.  dp_cur[0] is always 1 (empty partition). */
    for (int p = (int)N; p >= 2; --p) {
        dp_cur[0] = (seq_t)1;
        for (size_t s = 1; s <= (size_t)N; ++s) {
            seq_t val = dp_prev[s];                     /* partitions using parts > p */
            if (s >= (size_t)p) val = val + dp_cur[s - p]; /* add a part of size p */
            dp_cur[s] = val;
        }

        /* For all n with smallest prime factor equal to p, store answer. */
        for (uint32_t n = head[p]; n != 0; n = next[n]) {
            cache[(size_t)n] = dp_cur[(size_t)n];
        }

        /* Prepare for next iteration: dp_prev becomes dp_cur. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    /* Handle n = 1 separately (a(1) = 1). */
    cache[1] = (seq_t)1;

    /* Free temporary arrays.  cache remains for future calls. */
    free(spf);
    free(head);
    free(next);
    free(dp_prev);
    free(dp_cur);

    /* Update maximum cached value. */
    max_cached = N;
}

/* ---------- public API ---------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* safety; the sequence is defined for n≥1 */
    if (n > max_cached) compute_all(n);
    return cache[(size_t)n];
}
