/*
 * A209192  Numbers k such that digital root of k equals largest prime factor
 *          of k.
 * https://oeis.org/A209192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that digital_root(k) *
 *            equals the largest prime factor of k. All such k are
 *            2,3,5,7‑smooth * numbers whose largest prime factor is a prime
 *            in {2,3,5,7}. We generate * all 2,3,5,7‑smooth numbers in
 *            increasing order and keep those that satisfy * the conditio...
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 200, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- helper: digital root of a non‑zero x ---------- */
static inline seq_t digital_root(seq_t x)
{
    seq_t sum = 0;
    seq_t y   = x;
    while (y > 0) {
        sum += y % 10;
        y   /= 10;
    }
    seq_t dr = sum % 9;
    return (dr == 0) ? 9 : dr;
}

/* ---------- helper: largest prime factor for 2,3,5,7‑smooth numbers ---------- */
static inline seq_t largest_of_smooth(seq_t x)
{
    if (x % 7 == 0) return 7;
    if (x % 5 == 0) return 5;
    if (x % 3 == 0) return 3;
    return 2;
}

/* ---------- static containers ---------- */
#ifdef __cplusplus
static std::vector<seq_t> cache;   /* collected sequence terms */
static std::vector<seq_t> smooth;  /* all 2,3,5,7‑smooth numbers generated so far */
static size_t p2 = 0, p3 = 0, p5 = 0, p7 = 0; /* indices for humble generation */
#else
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;

static seq_t *smooth    = NULL;
static size_t smooth_cap = 0;
static size_t smooth_sz  = 0;

static size_t p2 = 0, p3 = 0, p5 = 0, p7 = 0;

static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, but not used */

#ifdef __cplusplus
    if (smooth.empty()) {
        smooth.push_back(1);
    }
#else
    if (smooth_sz == 0) {
        ensure_cap(&smooth, &smooth_cap, 8);
        smooth[0] = 1;
        smooth_sz = 1;
    }
#endif

    /* Generate terms until we have at least n values in cache */
    while (
#ifdef __cplusplus
        cache.size() < (size_t)n
#else
        cache_sz  < (size_t)n
#endif
    ) {

        /* ----- generate next humble (2,3,5,7‑smooth) number ----- */
        seq_t cand2 = smooth[p2] * 2;
        seq_t cand3 = smooth[p3] * 3;
        seq_t cand5 = smooth[p5] * 5;
        seq_t cand7 = smooth[p7] * 7;

        seq_t nxt = cand2;
        if (cand3 < nxt) nxt = cand3;
        if (cand5 < nxt) nxt = cand5;
        if (cand7 < nxt) nxt = cand7;

        if (cand2 == nxt) ++p2;
        if (cand3 == nxt) ++p3;
        if (cand5 == nxt) ++p5;
        if (cand7 == nxt) ++p7;

#ifdef __cplusplus
        smooth.push_back(nxt);
#else
        ensure_cap(&smooth, &smooth_cap, smooth_sz + 1);
        smooth[smooth_sz++] = nxt;
#endif

        if (nxt == (seq_t)1) continue;   /* skip dummy 1 */

        /* ----- test the condition ----- */
        seq_t dr = digital_root(nxt);
        seq_t lpf = largest_of_smooth(nxt);
        if (dr == lpf) {
#ifdef __cplusplus
            cache.push_back(nxt);
#else
            ensure_cap(&cache, &cache_cap, cache_sz + 1);
            cache[cache_sz++] = nxt;
#endif
        }
    }

    /* return the n‑th term (1‑based index) */
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
