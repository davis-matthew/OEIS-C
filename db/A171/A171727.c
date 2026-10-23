/*
 * A171727  The number of twin prime pairs in the interval (p^2,p*q), where
 *          (p,q) runs over the twin prime pairs (A001359(n),A006512(n)).
 * https://oeis.org/A171727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 585, u16 n <= 1044, u32 n <= 1044, u64 n <= 1044,
 *            u128 n <= 1044, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of twin prime pairs in (p^2,p*q) where (p,q) runs over
 * the twin prime pairs (A001359(n),A006512(n)).
 * a(n) counts twin primes t,t+2 such that p^2 < t < t+2 < p*q.
 * The algorithm pre‑computes twin primes up to a dynamic limit until
 * it contains at least n pairs, builds a segmented sieve for each
 * interval, and counts the twin pairs inside.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"    /* void build_primes_upto(idx_t limit,
                                            uint64_t **primes_out,
                                            size_t *cnt_out); */

static uint64_t *g_primes = NULL;     /* primes up to current_limit */
static size_t    g_primes_cnt = 0;    /* number of primes */
static uint64_t *g_twin_p = NULL;     /* first member of each twin pair */
static uint64_t *g_twin_q = NULL;     /* second member of each twin pair */
static size_t    g_twin_cnt = 0;      /* number of twin pairs */
static idx_t     g_limit = 4000000;   /* current sieve limit */
static bool      g_initialized = false;

/* Allocated segment buffer reused for all calls */
static char *g_seg_buf = NULL;
static size_t g_seg_buf_cap = 0;

/* Helper to build the primes and twin prime lists up to g_limit. */
static void rebuild_sequence(void)
{
    if (g_primes) free(g_primes);
    if (g_twin_p) free(g_twin_p);
    if (g_twin_q) free(g_twin_q);

    /* Build primes up to g_limit */
    build_primes_upto(g_limit, &g_primes, &g_primes_cnt);

    /* Count twin pairs */
    size_t cnt = 0;
    for (size_t i = 0; i + 1 < g_primes_cnt; ++i) {
        if (g_primes[i + 1] - g_primes[i] == 2) ++cnt;
    }
    g_twin_cnt = cnt;
    g_twin_p = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    g_twin_q = (uint64_t *)malloc(cnt * sizeof(uint64_t));

    /* Store twin pairs */
    size_t idx = 0;
    for (size_t i = 0; i + 1 < g_primes_cnt; ++i) {
        if (g_primes[i + 1] - g_primes[i] == 2) {
            g_twin_p[idx] = g_primes[i];
            g_twin_q[idx] = g_primes[i + 1];
            ++idx;
        }
    }
}

/* Ensure we have at least n twin pairs pre‑computed. */
static void ensure_twins(idx_t n)
{
    if (!g_initialized) {
        rebuild_sequence();
        g_initialized = true;
        return;
    }
    while ((idx_t)g_twin_cnt < n) {
        g_limit *= 2;          /* increase sieve limit */
        rebuild_sequence();
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_twins(n);

    uint64_t p = g_twin_p[n - 1];
    uint64_t q = g_twin_q[n - 1];

    uint64_t low  = p * p;          /* inclusive lower bound of interval */
    uint64_t high = p * q;          /* exclusive upper bound of interval */

    if (high - low <= 1) return (seq_t)0;      /* empty interval */

    uint64_t seg_start = low + 1;
    uint64_t seg_end   = high - 1;            /* inclusive */

    size_t seg_len = (size_t)(seg_end - seg_start + 1);
    if (g_seg_buf_cap < seg_len) {
        free(g_seg_buf);
        g_seg_buf = (char *)malloc(seg_len);
        g_seg_buf_cap = seg_len;
    }
    memset(g_seg_buf, 0, seg_len);

    /* Mark composites in the segment using precomputed primes. */
    for (size_t i = 0; i < g_primes_cnt; ++i) {
        uint64_t pr = g_primes[i];
        uint64_t pr_sq = pr * pr;
        if (pr_sq > seg_end) break;

        uint64_t first = seg_start;
        uint64_t r = first % pr;
        if (r != 0) first += pr - r;
        if (first < pr_sq) first = pr_sq;   /* start at prime square if in segment */

        for (uint64_t x = first; x <= seg_end; x += pr) {
            g_seg_buf[(size_t)(x - seg_start)] = 1;   /* composite */
        }
    }

    /* Count twin prime pairs in the interval. */
    uint64_t count = 0;
    uint64_t t = seg_start;
    if ((t & 1ULL) == 0) ++t;                 /* ensure t is odd */

    for (; t + 2 <= seg_end; t += 2) {
        if (!g_seg_buf[(size_t)(t - seg_start)] &&
            !g_seg_buf[(size_t)(t + 2 - seg_start)]) {
            ++count;
        }
    }
    return (seq_t)count;
}
