/*
 * A079357  a(1)=1; a(n)=a(n-1)-1 if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A079357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1
   a(n)=a(n-1)-1 if integer n already occurs among {a(1),...,a(n-1)},
   else a(n)=a(n-1)+4.
   The sequence grows roughly like n + O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state: cache of computed a(i) and a bitmap of seen values */
    static seq_t *cache   = NULL;      /* cache[1..] holds a(i)           */
    static size_t cache_cap = 0;       /* allocated size of cache        */
    static bool  *seen   = NULL;       /* seen[v] == true iff value v has appeared */
    static size_t seen_cap  = 0;       /* allocated size of seen  */
    static idx_t  max_cached = 0;      /* highest index already cached   */

    /* --- initialise on first call --------------------------------- */
    if (cache_cap == 0) {
        cache_cap = 2;                         /* at least index 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */

        cache[1] = (seq_t)1;
        max_cached = 1;

        seen_cap = 128;                       /* initial seen capacity */
        seen = (bool *)calloc(seen_cap, sizeof(bool));
        if (!seen) return (seq_t)0;

        seen[1] = true;                       /* a(1)=1 is seen */
    }

    /* --- grow cache if needed ------------------------------------- */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- generate terms up to n ----------------------------------- */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* check whether integer i is already in the sequence */
        bool already = false;
        if (i < seen_cap) already = seen[i];

        seq_t cur = already ? prev - (seq_t)1 : prev + (seq_t)4;
        cache[i] = cur;

        /* enlarge seen table to hold index cur */
        size_t cur_idx = (size_t)cur;
        if (cur_idx >= seen_cap) {
            size_t new_cap = seen_cap ? seen_cap : 1;
            while (cur_idx >= new_cap) new_cap <<= 1;
            bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
            if (!tmp) return (seq_t)0;
            /* initialise new part of the array to false */
            memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
            seen = tmp;
            seen_cap = new_cap;
        }
        seen[cur_idx] = true;               /* mark that value cur now appears */
    }

    max_cached = (max_cached < n) ? n : max_cached;
    return cache[n];
}
