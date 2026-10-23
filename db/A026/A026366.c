/*
 * A026366  a(n) = a(m) if a(m) has already occurred exactly once and n =
 *          a(m)+2m, else a(n) = least positive integer that has not yet
 *          occurred.
 * https://oeis.org/A026366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A026366
   a(n) = a(m) if a(m) has occurred exactly once and n = a(m)+2m;
   else a(n) = least positive integer not yet occurred.
   Offset 1 (a(1) is the first term). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static arrays used for memoisation */
    static seq_t *cache = NULL;       /* computed a(k) for k=1..cache_size  */
    static size_t cache_cap = 0, cache_size = 0;
    static seq_t *dupe  = NULL;       /* dupe[pos-1] = value scheduled at position pos */
    static size_t dupe_cap = 0;
    static seq_t *occurrence = NULL;  /* occurrence[ val-1 ] = number of times val seen   */
    static size_t occ_cap = 0;
    static seq_t next_unused = 1;     /* smallest value not used yet */

    /* initialise static storage once */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));

        dupe_cap = 16;
        dupe = (seq_t*)calloc(dupe_cap, sizeof(seq_t));

        occ_cap = 16;
        occurrence = (seq_t*)calloc(occ_cap, sizeof(seq_t));
    }

    /* extend cache until it contains terms up to n */
    while (cache_size < n) {
        idx_t m = cache_size + 1;  /* 1‑based index of the term being generated */

        seq_t val;

        /* if a duplicate for this position has been pre‑scheduled use it */
        if (m < dupe_cap && dupe[m-1] != 0) {
            val = dupe[m-1];
        } else {
            /* find smallest not yet used */
            size_t idx = (size_t)next_unused;
            while (1) {
                if (idx > occ_cap) {
                    /* grow occurrence array to encompass idx */
                    size_t new_cap = occ_cap ? occ_cap * 2 : 16;
                    while (new_cap < idx) new_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(occurrence, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    memset(tmp + occ_cap, 0, (new_cap - occ_cap) * sizeof(seq_t));
                    occurrence = tmp;
                    occ_cap = new_cap;
                }
                if (occurrence[idx-1] == 0) {
                    val = (seq_t)idx;
                    break;
                }
                ++idx;
            }
            next_unused = (seq_t)(idx + 1);
        }

        /* update occurrence count for val */
        size_t val_idx = (size_t)val;
        if (val_idx > occ_cap) {
            size_t new_cap = occ_cap ? occ_cap * 2 : 16;
            while (new_cap < val_idx) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(occurrence, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            memset(tmp + occ_cap, 0, (new_cap - occ_cap) * sizeof(seq_t));
            occurrence = tmp;
            occ_cap = new_cap;
        }
        seq_t old_cnt = occurrence[val_idx - 1];
        occurrence[val_idx - 1] = old_cnt + 1;

        /* if this is the first occurrence, schedule the duplicate */
        if (old_cnt == 0) {
            /* target = val + 2 * m ; cast to seq_t to avoid type clash */
            seq_t target = val + ((seq_t)2 * (seq_t)m); /* target position for duplicate */
            size_t target_idx = (size_t)target;
            if (target_idx > dupe_cap) {
                size_t new_cap = dupe_cap ? dupe_cap * 2 : 16;
                while (new_cap < target_idx) new_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(dupe, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                memset(tmp + dupe_cap, 0, (new_cap - dupe_cap) * sizeof(seq_t));
                dupe = tmp;
                dupe_cap = new_cap;
            }
            dupe[target_idx - 1] = val;
        }

        /* store the generated term */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < cache_size + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[n-1];
}
