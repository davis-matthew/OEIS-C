/*
 * A171911  Van Eck's sequence (cf. A181391), but starting with a(1) = 1.
 * https://oeis.org/A171911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is Van Eck's sequence with a(1)=1, a(n+1) = min{ k>0 :
 *            a(n+1−k)=a(n) } or 0 if no such k
 * Range:     u8 n <= 381, u16 n <= 9725, u32 n <= 9725, u64 n <= 9725,
 *            u128 n <= 9725, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* the build provides these types */
extern seq_t a(idx_t n);          /* required prototype */

/* internal static state */
static seq_t *seq_cache      = NULL;   /* seq_cache[i] = a(i+1)  (0‑based)   */
static size_t seq_cache_len  = 0;      /* number of computed terms          */
static size_t seq_cache_cap  = 0;      /* allocated capacity of seq_cache   */

static idx_t *last_occ       = NULL;   /* last_occ[value] = last index (1‑based) where value occurred */
static size_t last_cap       = 0;       /* capacity of last_occ array         */

/* ensure seq_cache has capacity 'need' (at least) */
static void seq_ensure_cache(idx_t need)
{
    if (seq_cache_cap < (size_t)need) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 4;
        if (new_cap < (size_t)need) new_cap = (size_t)need;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
}

/* ensure last_occ can index up to 'needed_val' (0‑based) */
static void seq_ensure_last(idx_t needed_val)
{
    if (last_cap <= (size_t)needed_val) {
        size_t new_cap = last_cap ? last_cap * 2 : 4;
        while (new_cap <= (size_t)needed_val) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(last_occ, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        last_occ = tmp;
        /* initialize newly allocated part to 0 */
        for (size_t i = last_cap; i < new_cap; ++i) last_occ[i] = 0;
        last_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1; 0 is safe */

    /* make sure we can address a(n) */
    seq_ensure_cache(n);

    /* initialise first term if needed */
    if (seq_cache_len == 0) {
        seq_cache[0] = (seq_t)1;          /* a(1)=1 */
        seq_cache_len = 1;
        seq_ensure_last(1);               /* ensure we can store value 1 */
        last_occ[1] = 1;                  /* last occurrence of 1 is at index 1 */
    }

    /* extend cache up to term n */
    while (seq_cache_len < n) {
        idx_t pos = seq_cache_len + 1;          /* position of next term (1‑based) */
        seq_t prev_val = seq_cache[seq_cache_len - 1];   /* a(pos-1) */

        /* ensure last_occ can index prev_val */
        seq_ensure_last((idx_t)prev_val);

        idx_t old_last = last_occ[(idx_t)prev_val];     /* last occurrence before pos-1 */
        seq_t next_val = (old_last == 0) ? (seq_t)0 : (seq_t)( (idx_t)(pos - 1) - old_last );

        seq_cache[seq_cache_len] = next_val;   /* store a(pos) */
        seq_cache_len++;

        /* update last_occ for prev_val to its most recent occurrence (pos-1) */
        last_occ[(idx_t)prev_val] = pos - 1;
    }

    return seq_cache[n - 1];
}
