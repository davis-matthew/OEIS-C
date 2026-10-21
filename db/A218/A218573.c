/*
 * A218573  Number of partitions p of n such that max(p) - min(p) = 10.
 * https://oeis.org/A218573
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of integer partitions of n whose largest part
 *            minus smallest part equals 10. For each possible smallest part s
 *            ≥ 1 we remove one part of size s and one part of size s+10
 *            (ensuring the difference is 10). Let m = n – (2s + 10). The
 *            remaining parts may be any number of parts chose...
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 216, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#if defined(__cplusplus)
#  include <cstdint>
#endif

/* Utility: allocation helpers that work in both C and C++ */
#if defined(__cplusplus)
static seq_t* alloc_seq(idx_t sz) { return new seq_t[sz](); }
static void   free_seq  (seq_t* p) { delete[] p; }
#else
static seq_t* alloc_seq(idx_t sz)
{
    seq_t* p = (seq_t*)malloc(sz * sizeof(seq_t));
    if (p) memset(p,0,sz*sizeof(seq_t));
    return p;
}
static void   free_seq  (seq_t* p)
{
    free(p);
}
#endif

/* cache for a(n) */
static seq_t* seq_cache   = NULL;  /* array of a(n) values, index is n */
static idx_t  cache_limit = 0;    /* cache_limit == size of seq_cache */

/* recompute a(n) for all n up to limit (inclusive) */
static void recompute(idx_t limit)
{
    if (limit < 12) return;                 /* no values needed below offset */

    /* allocate new cache, free old one */
    if (seq_cache) free_seq(seq_cache);
    seq_cache   = alloc_seq(limit + 1);
    cache_limit = limit + 1;

    /* initialise cache to zero; already zeroed by alloc_seq() */

    /* temporary array dp for coin change */
    seq_t* dp = alloc_seq(limit + 1);

    /* maximum possible smallest part */
    idx_t s_max = (limit - 10) / 2;   /* floor */

    for (idx_t s = 1; s <= s_max; ++s)
    {
        idx_t sub = 2*s + 10;             /* n - 2s - 10 */
        if (sub > limit) continue;        /* should not happen */

        /* initialise dp[0] = 1, others 0 */
        memset(dp, 0, (limit + 1) * sizeof(seq_t));
        dp[0] = 1;

        /* coin change: coins are s .. s+10 */
        for (idx_t coin = s; coin <= s + 10; ++coin)
        {
            for (idx_t t = coin; t <= limit; ++t)
            {
                dp[t] += dp[t - coin];
            }
        }

        /* accumulate contributions to a(n) */
        idx_t start_n = sub;   /* smallest n that this s can contribute to */
        for (idx_t n = start_n; n <= limit; ++n)
        {
            seq_t val = dp[n - sub];
            seq_cache[n] += val;
        }
    }

    free_seq(dp);
}

/* public function to be used by the grading framework */
seq_t a(idx_t n)
{
    if (n < 12) return (seq_t)0;     /* values below offset are undefined */

    /* recompute cache if needed */
    if (n >= cache_limit) recompute(n);

    return seq_cache[n];
}
