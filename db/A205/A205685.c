/*
 * A205685  The number j such that 5 divides prime(k)-prime(j), where
 *          k(n)=A205684(n).
 * https://oeis.org/A205685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j-th index, listing for each k=2,3,... (prime(k) index)
 *            the j<k with prime(j) == prime(k) (mod 5), j ascending; computed
 *            by incremental prime generation and counting
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *g_pr = NULL;      /* primes */
static uint64_t *g_cum = NULL;     /* g_cum[k-1] = terms through prime index k */
static uint32_t *g_lists[5] = {NULL, NULL, NULL, NULL, NULL};
static size_t g_lcnt[5] = {0, 0, 0, 0, 0};
static size_t g_lcap[5] = {0, 0, 0, 0, 0};
static size_t g_sz = 0, g_cap = 0;

static void A205685_extend(void)
{
    uint32_t p;
    if (g_sz == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 64;
        g_pr = (uint32_t *)realloc(g_pr, g_cap * sizeof(uint32_t));
        g_cum = (uint64_t *)realloc(g_cum, g_cap * sizeof(uint64_t));
        if (!g_pr || !g_cum) abort();
    }
    if (g_sz == 0) p = 2;
    else {
        p = g_pr[g_sz - 1] + (g_sz == 1 ? 1 : 2);
        for (;; p += 2) {
            bool ok = true;
            for (size_t i = 0; i < g_sz; ++i) {
                uint32_t q = g_pr[i];
                if ((uint64_t)q * q > p) break;
                if (p % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
    }
    g_pr[g_sz] = p;
    unsigned r = p % 5U;
    uint64_t prev = g_sz ? g_cum[g_sz - 1] : 0;
    g_cum[g_sz] = prev + g_lcnt[r];
    if (g_lcnt[r] == g_lcap[r]) {
        g_lcap[r] = g_lcap[r] ? g_lcap[r] * 2 : 16;
        g_lists[r] = (uint32_t *)realloc(g_lists[r], g_lcap[r] * sizeof(uint32_t));
        if (!g_lists[r]) abort();
    }
    g_lists[r][g_lcnt[r]++] = (uint32_t)(g_sz + 1);
    ++g_sz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (g_sz == 0 || g_cum[g_sz - 1] < (uint64_t)n) A205685_extend();
    /* binary search smallest index i with g_cum[i] >= n */
    size_t lo = 0, hi = g_sz - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (g_cum[mid] >= (uint64_t)n) hi = mid; else lo = mid + 1;
    }
    uint64_t before = lo ? g_cum[lo - 1] : 0;
    size_t t = (size_t)((uint64_t)n - before - 1);
    unsigned r = g_pr[lo] % 5U;
    return (seq_t)g_lists[r][t];
}
