/*
 * A141463  Conjectured to be the complete list of numbers not of the form
 *          2*x^2 + 5*y^2 + 4*triangular number.
 * https://oeis.org/A141463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers not representable as 2*x^2 + 5*y^2 + 4*T_k
   (T_k = k*(k+1)/2).  The implementation keeps a cache of the sequence
   values and, incrementally, the set of all representable values up to
   the largest number examined.  The algorithm runs in about O(N·sqrt(N))
   time and is fast enough for n up to 10 000. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Global state used to cache results between calls                       */
/* ------------------------------------------------------------------ */
static seq_t *cache        = NULL;          /* cache of sequence values */
static size_t cache_len    = 0;             /* number of cached values */

static idx_t next_candidate = 1;            /* next integer to test */

static unsigned char *rep      = NULL;      /* bitset for 2x^2+5y^2 <= m */
static idx_t rep_max   = 0;                 /* current maximum for rep[] */

static idx_t *tri4          = NULL;        /* 4*T_k values, increasing */
static size_t tri4_count    = 0;            /* number of stored values   */
static size_t tri4_cap      = 0;            /* allocated capacity       */
static idx_t  tri4_last_k   = 0;            /* last k processed          */

/* ------------------------------------------------------------------ */
/* Ensure rep[] can index up to 'limit'.  The array is grown to the
   next power of two greater than 'limit' and all sums 2x^2+5y^2 <= limit
   are recomputed.  ------------------------------------------------------------------ */
static void ensure_rep_max(idx_t limit) {
    if (limit <= rep_max)
        return;

    idx_t new_max = rep_max ? rep_max * 2 : 1;
    while (new_max < limit)
        new_max *= 2;

    rep = (unsigned char*)realloc(rep, (size_t)new_max + 1);
    if (!rep)
        abort();   /* out of memory */

    /* zero new portion */
    memset(rep + rep_max + 1, 0, (size_t)(new_max - rep_max));

    /* recompute all sums <= new_max */
    for (idx_t x = 0; (idx_t)2 * x * x <= new_max; ++x) {
        idx_t val2 = 2 * x * x;
        for (idx_t y = 0; val2 + 5 * y * y <= new_max; ++y) {
            rep[val2 + 5 * y * y] = 1;
        }
    }

    rep_max = new_max;
}

/* ------------------------------------------------------------------ */
/* Ensure tri4[] contains all values 4*T_k <= limit.                  */
/* ------------------------------------------------------------------ */
static void ensure_tri4(idx_t limit) {
    while (true) {
        idx_t k = tri4_last_k;
        unsigned long long T = (unsigned long long)k * (k + 1) / 2;
        unsigned long long val = 4 * T;
        if (val > limit)
            break;

        if (tri4_count == tri4_cap) {
            tri4_cap = tri4_cap ? tri4_cap * 2 : 128;
            tri4 = (idx_t*)realloc(tri4, tri4_cap * sizeof(idx_t));
            if (!tri4)
                abort();
        }
        tri4[tri4_count++] = (idx_t)val;
        ++tri4_last_k;
    }
}

/* ------------------------------------------------------------------ */
/* Main function computing the n‑th term (n >= 1).                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;        /* undefined for offset 1 */

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t m = next_candidate++;
        ensure_rep_max(m);
        ensure_tri4(m);

        bool is_rep = false;
        for (size_t i = 0; i < tri4_count; ++i) {
            idx_t t = tri4[i];
            if (t > m)
                break;
            idx_t rem = m - t;
            if (rep[rem]) {
                is_rep = true;
                break;
            }
        }

        if (!is_rep) {
            cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            if (!cache)
                abort();
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
