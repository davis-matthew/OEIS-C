/*
 * A050136  a(n) = floor(a(n-1)/2) if this is not among 0,a(1),...,a(n-1),
 *          otherwise a(n)=5*n.
 * https://oeis.org/A050136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A050136
 *  a(1) = 1
 *  For n > 1:
 *      let h = floor(a(n-1)/2)
 *      if h has never appeared before (including 0) then a(n) = h
 *      else a(n) = 5*n
 */
 
#include <stdlib.h>
#include <stdbool.h>
 
/* local static state ------------------------------------------------------- */
static seq_t *cache      = NULL;   /* cache of computed a(n), 0‑based index */
static idx_t  cache_len  = 0;      /* number of terms stored */
static bool  *seen       = NULL;   /* bitset: seen[v] == true iff v appeared */
static idx_t  seen_size  = 0;      /* current capacity of the bitset            */
 
/* ensure that the bitset has room for values up to 'need' (inclusive) */
static void ensure_seen_size(idx_t need)
{
    if (need < seen_size) return;
    idx_t newSize = need + 1;
    if (newSize < seen_size * 2 + 10) newSize = seen_size * 2 + 10;
    bool *tmp = (bool *)realloc(seen, newSize * sizeof(bool));
    for (idx_t j = seen_size; j < newSize; ++j) tmp[j] = false;
    seen = tmp;
    seen_size = newSize;
}
 
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused; protects against n<1 */
 
    /* initialise seen[] the first time we are called   */
    if (seen == NULL) {
        seen_size = 1;
        seen = (bool *)malloc(sizeof(bool));
        seen[0] = true;            /* 0 is considered already seen   */
    }
 
    /* compute terms up to n (inclusive) if they are not yet present         */
    while (cache_len < n) {
        idx_t i = cache_len + 1;    /* 1‑based index of the new term     */
        seq_t val;
 
        if (cache_len == 0) {                      /* a(1) */
            val = (seq_t)1;
        } else {
            seq_t half   = cache[cache_len - 1] / (seq_t)2;
            idx_t h_idx = (idx_t)half;
            bool notSeen;
            if (h_idx < seen_size) notSeen = !seen[h_idx];
            else notSeen = true;                   /* cannot be seen yet  */
            val = notSeen ? half : (seq_t)i * (seq_t)5;
        }
 
        /* expand cache if necessary */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        cache[cache_len] = val;
 
        /* mark this value as seen for future lookups */
        idx_t val_idx = (idx_t)val;
        ensure_seen_size(val_idx);
        seen[val_idx] = true;
 
        cache_len++;
    }
 
    return cache[n - 1];
}
