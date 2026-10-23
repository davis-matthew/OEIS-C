/*
 * A031561  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 63.
 * https://oeis.org/A031561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that the continued fraction of sqrt(k) has an even period
   and the central term of that period equals 63.  The function generates the
   sequence by incremental search starting from k = 2.  It works for any
   seq_t size (unsigned 8,16,32,64,128 bits or arbitrary precision). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root: floor(sqrt(n)).  Uses a bit‑shift algorithm that
   never overflows for 64‑bit unsigned values. */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t r = 1ULL << (63 - __builtin_clzll(n));   /* 2^⌊log2(√n)⌋ */
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Dynamic array growth helper used for the cache of found k values. */
static void ensure_cap(size_t *cap, void **ptr, size_t elem_size, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap *= 2;
        void *tmp = realloc(*ptr, newcap * elem_size);
        *ptr = tmp;
        *cap = newcap;
    }
}

/* Static buffer large enough for any period we encounter (period ≤ 2·√k).
   For k up to 10^12 the period is < 2·10^6, well below this limit.   */
static int period[65536];

/* Test whether a given k satisfies the sequence condition.
   On success *center will contain the central term of the period. */
static bool qualifies(uint64_t k, int *center)
{
    uint64_t a0 = floor_sqrt_u64(k);
    /* Skip perfect squares */
    if ( (uint64_t)((__int128)a0 * a0) == k ) return false;

    uint64_t m = 0, d = 1, a = a0;
    size_t len = 0;
    while (1) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        period[len++] = (int)a;
        if (a == 2 * a0) break;               /* period closed */
    }
    if (len & 1) return false;                /* odd period length */
    *center = period[len/2 - 1];              /* central term (0‑based) */
    return (*center == 63);
}

/* Global state for incremental generation of the sequence. */
static seq_t  *cache     = NULL;   /* cached values */
static size_t  cache_len = 0;      /* number of cached values */
static size_t  cache_cap = 0;      /* allocated capacity */
static uint64_t next_k   = 2;      /* next integer to test */

/* Find the next qualifying k and store it in *val. */
static void locate_next(uint64_t *val)
{
    for (;;) {
        uint64_t k = next_k++;
        int  center;
        if (qualifies(k, &center)) {
            *val = k;
            return;
        }
    }
}

/* Ensure at least n values are cached. */
static void compute_upto(size_t n)
{
    while (cache_len < n) {
        uint64_t v;
        locate_next(&v);
        ensure_cap(&cache_cap, (void **)&cache, sizeof(seq_t), cache_len + 1);
        cache[cache_len++] = (seq_t)v;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* below offset, value uninteresting */
    compute_upto((size_t)n);
    return cache[(size_t)n - 1];
}
