/*
 * A075915  Seventh column of triangle A075500.
 * https://oeis.org/A075915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * Stirling2(n+7, 7). Recurrence: a(n) = 140 a(n‑1) -
 *            8050 a(n‑2) + 245000 a(n‑3) - 4230625 a(n‑4) + 41037500 a(n‑5) -
 *            204187500 a(n‑6) + 393750000 a(n‑7)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values for n = 0 … 6 are given. */

#include <stdlib.h>

/* Elementary symmetric sums of 5,10,15,20,25,30,35.  */
static const seq_t E1 = (seq_t)140;
static const seq_t E2 = (seq_t)8050;
static const seq_t E3 = (seq_t)245000;
static const seq_t E4 = (seq_t)4230625;
static const seq_t E5 = (seq_t)41037500;
static const seq_t E6 = (seq_t)204187500;
static const seq_t E7 = (seq_t)393750000;

/* Cached sequence values.  */
static seq_t *cache = NULL;           /* array of a(i) for 0 <= i < cache_len */
static size_t cache_len = 0;          /* number of elements stored      */

/* Ensure that cache holds values up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;        /* at least n+1 elements        */

    /* if cache empty: allocate the minimal size (at least 7).  */
    if (cache == NULL) {
        if (need < 7) need = 7;           /* have room for initial 7 terms */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (cache == NULL) return;       /* allocation failed – caller will
                                            be unable to compute value */

        cache_len = need;
        /* initialise the first seven terms that are known.  */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)140;
        cache[2] = (seq_t)11550;
        cache[3] = (seq_t)735000;
        cache[4] = (seq_t)39991875;
        cache[5] = (seq_t)1960612500;
        cache[6] = (seq_t)89303500000;

        /* extend cache if more than 7 entries are requested.  */
        if (need > 7) {
            size_t i;
            for (i = 7; i < need; ++i) {
                seq_t val = E1 * cache[i - 1];
                val -= E2 * cache[i - 2];
                val += E3 * cache[i - 3];
                val -= E4 * cache[i - 4];
                val += E5 * cache[i - 5];
                val -= E6 * cache[i - 6];
                val += E7 * cache[i - 7];
                cache[i] = val;
            }
        }
    }
    /* if existing cache is too short, extend it.  */
    else if (cache_len <= (size_t)n) {
        /* grow to at least n+1, or double the size for amortised
           linear behaviour – simple approach is to allocate exactly n+1.  */
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (new_cache == NULL) return;  /* allocation failed – caller fails */

        cache = new_cache;
        /* compute new entries.  */
        size_t i;
        for (i = cache_len; i <= (size_t)n; ++i) {
            seq_t val = E1 * cache[i - 1];
            val -= E2 * cache[i - 2];
            val += E3 * cache[i - 3];
            val -= E4 * cache[i - 4];
            val += E5 * cache[i - 5];
            val -= E6 * cache[i - 6];
            val += E7 * cache[i - 7];
            cache[i] = val;
        }
        cache_len = need;
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have enough pre‑computed values.  */
    ensure_cache(n);
    if (cache == NULL) return (seq_t)0;   /* allocation failed */
    return cache[(size_t)n];
}
