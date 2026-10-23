/*
 * A147562  Number of "ON" cells at n-th stage in the "Ulam-Warburton"
 *          two-dimensional cellular automaton.
 * https://oeis.org/A147562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0)=0, a(1)=1, a(n) = a(n-1) + 4 * 3^(popcount(n-1)-1) for n ≥
 *            2 (equivalently a(n) = 1 + 4·∑_{k=1}^{n-1} 3^(popcount(k)-1) for
 *            n>1)
 * Range:     u8 n <= 15, u16 n <= 247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>

/* Max index that is safely stored in the static cache.  Adjust if a larger
   range is required; the algorithm works for any n, but values beyond this
   limit will be computed without being cached. */
#define MAX_CACHE  200000

/* Static cache of computed terms */
static seq_t     cache[MAX_CACHE + 1];
static idx_t      max_computed = 0;   /* highest index for which a[i] is stored */

/* Pre‑computed powers of 3 up to 3^64 (popcount of 64‑bit integer ≤ 64) */
static seq_t      pow3[65];
static int        pow3_inited = 0;

/* Initialise the powers-of‑3 table once */
static void init_pow3(void)
{
    if (pow3_inited) return;
    pow3[0] = (seq_t)1;
    for (int i = 1; i < 65; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;
    pow3_inited = 1;
}

/* Compute terms up to (and including) index n, storing them if within the
   static cache bounds.  Values beyond MAX_CACHE are still computed, but
   not retained. */
static void ensure_cache(idx_t n)
{
    if (max_computed >= n) return;    /* already computed */

    init_pow3();

    /* Initialise base values if the cache is empty */
    if (max_computed == 0) {
        /* a(0) = 0 */
        cache[0] = (seq_t)0;
        if (n >= 1) {                 /* a(1) = 1 */
            cache[1] = (seq_t)1;
            max_computed = 1;
        } else {                      /* n == 0 */
            max_computed = 0;
            return;
        }
    }

    /* Continue computing from the next index */
    seq_t prev = cache[max_computed];
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term;
        if (i == 1) {                  /* should not happen after init, but keep safe */
            term = 0;
            cache[1] = (seq_t)1;
            prev = cache[1];
            max_computed = 1;
            continue;
        }
        /* popcount of (i-1) – guaranteed ≥ 1 for i ≥ 2 */
        int pc = __builtin_popcountll((uint64_t)(i - 1));
        term = pow3[pc - 1];           /* 3^(popcount-1) */

        seq_t cur = prev + (seq_t)4 * term;
        if (i <= MAX_CACHE) {
            cache[i] = cur;
            prev = cur;
            max_computed = i;
        } else {
            /* Beyond cached limits – compute on the fly */
            prev = cur;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    ensure_cache(n);
    if (n <= MAX_CACHE)
        return cache[n];
    /* If n > MAX_CACHE, recompute just the final value (no cache) */
    seq_t value = cache[max_computed];
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        int pc = __builtin_popcountll((uint64_t)(i - 1));
        seq_t term = pow3[pc - 1];
        value = value + (seq_t)4 * term;
    }
    return value;
}
