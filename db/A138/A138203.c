/*
 * A138203  Sieve: start with the natural numbers; at the n-th step in the
 *          sieve remove every A138204(n)-th term from the sequence remaining
 *          in the prior step, where A138204 is the complement to this
 *          sequence starting with A138204(1)=2.
 * https://oeis.org/A138203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A138203 computed by an iterative sieve. The step values are the
 *            smallest removed number greater than the previous step. The
 *            survivor list is obtained by repeatedly deleting every k‑th term
 *            where k is the current step. The sequence a(n) (1‑indexed) is
 *            the n‑th element of that final survivor list
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static storage for the cache of computed survivors */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static bool cache_ready = false;

/* Helper function that builds the survivor cache large enough
   to contain at least 'need' terms.  It reallocates a new cache
   if the current one is insufficient. */
static void build_cache(idx_t need) {
    idx_t limit = 200000;                  /* initial guess */
    if (limit < need * 20) limit = need * 20;

    while (true) {
        /* allocate arrays for the sieve */
        seq_t *cur  = (seq_t*)malloc(sizeof(seq_t) * limit);
        seq_t *nxt  = (seq_t*)malloc(sizeof(seq_t) * limit);
        char  *rem  = (char*)calloc(limit + 1, sizeof(char));  /* removed flags */
        if (!cur || !nxt || !rem) abort();

        /* fill the initial list 1..limit */
        for (idx_t i = 1; i <= limit; ++i) cur[i - 1] = (seq_t)i;
        idx_t cur_len = limit;

        idx_t step = 2;                    /* first step value */

        /* perform the sieve */
        while (true) {
            idx_t nxt_len = 0;
            for (idx_t i = 0; i < cur_len; ++i) {
                /* positions are 1‑based */
                idx_t pos = i + 1;
                seq_t v   = cur[i];
                if (pos % step != 0) {
                    /* keep the element */
                    nxt[nxt_len++] = v;
                } else {
                    /* element is removed */
                    rem[(idx_t)v] = 1;
                }
            }

            /* swap buffers and update length */
            seq_t *tmp = cur; cur = nxt; nxt = tmp;
            cur_len = nxt_len;

            /* find the next step: smallest removed number > previous step */
            ++step;
            while (step <= limit && rem[step] == 0) ++step;

            if (step > cur_len) break;    /* no further deletions possible */
        }

        /* allocate cache and copy the survivors */
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * cur_len);
        memcpy(new_cache, cur, sizeof(seq_t) * cur_len);
        free(cur); free(nxt); free(rem);

        if (cur_len >= need) {
            /* store in the static cache */
            if (cache) free(cache);
            cache     = new_cache;
            cache_len = cur_len;
            cache_ready = true;
            return;
        }
        /* not enough survivors: enlarge the limit and retry */
        free(new_cache);
        limit *= 2;
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    if (!cache_ready || n > cache_len) {
        build_cache(n);
    }
    return cache[n - 1];  /* 1‑based indexing */
}
