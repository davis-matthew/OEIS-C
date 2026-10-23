/*
 * A131845  a(1)=a(2)=1; a(n) = number of terms of the sequence (from among
 *          terms a(1) through a(n-1)) that divide (a(n-1)*a(n-2)).
 * https://oeis.org/A131845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms among a(1), …, a(n‑1) that divide a(n‑1)
 *            * a(n‑2); a(1)=a(2)=1
 * Range:     u8 n <= 41, u16 n <= 1141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Sequences cache – 0‑based index, cache_len = number of stored terms */
static seq_t *cache          = NULL;
static idx_t  cache_len      = 0;
static idx_t  cache_cap      = 0;

/* Frequency table for the values seen so far */
static seq_t *freq_vals      = NULL;   /* distinct values */
static seq_t *freq_cnts      = NULL;   /* how many times each occurs */
static size_t freq_len       = 0;       /* number of distinct values */
static size_t freq_cap       = 0;       /* allocated capacity for the two arrays */

/* Ensure the cache can hold at least 'need' elements (1‑based).  */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need) new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Ensure the frequency table can hold at least one more distinct value. */
static void ensure_freq_capacity(void)
{
    if (freq_len < freq_cap) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 4;
    freq_vals = (seq_t *)realloc(freq_vals, new_cap * sizeof(seq_t));
    freq_cnts = (seq_t *)realloc(freq_cnts, new_cap * sizeof(seq_t));
    freq_cap  = new_cap;
}

/* Initialize the cache with the first two terms and the frequency table. */
static void init_sequence(void)
{
    /* Cache: a(1)=1, a(2)=1 */
    cache_cap = 4;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)1;   /* a(1) */
    cache[1] = (seq_t)1;   /* a(2) */
    cache_len = 2;

    /* Frequency table: value 1 occurs twice */
    freq_cap = 4;
    freq_vals = (seq_t *)malloc(freq_cap * sizeof(seq_t));
    freq_cnts = (seq_t *)malloc(freq_cap * sizeof(seq_t));
    freq_vals[0] = (seq_t)1;
    freq_cnts[0] = (seq_t)2;   /* two occurrences of 1 */
    freq_len = 1;
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term (1‑based) of sequence A131845.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for 0, but safe */
    if (cache_len == 0) init_sequence();

    if (n <= cache_len) return cache[n - 1];

    /* Make sure the cache has room for all terms up to n */
    ensure_cache_capacity(n);

    /* Compute missing terms up to n */
    while (cache_len < n) {
        /* product of the two previous terms */
        seq_t prod = cache[cache_len - 1] * cache[cache_len - 2];

        /* Count how many previous values divide prod */
        seq_t cnt = (seq_t)0;
        for (size_t p = 0; p < freq_len; ++p) {
            if (prod % freq_vals[p] == 0) {
                cnt += freq_cnts[p];
            }
        }

        /* Store the new term */
        cache[cache_len] = cnt;
        ++cache_len;

        /* Update frequency table for the new term */
        size_t pos = 0;
        while (pos < freq_len && freq_vals[pos] != cnt) ++pos;
        if (pos < freq_len) {
            /* Existing value – increment its count */
            freq_cnts[pos] = freq_cnts[pos] + (seq_t)1;
        } else {
            /* New distinct value */
            ensure_freq_capacity();
            freq_vals[freq_len] = cnt;
            freq_cnts[freq_len] = (seq_t)1;
            ++freq_len;
        }
    }

    return cache[n - 1];
}
