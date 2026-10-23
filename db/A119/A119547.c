/*
 * A119547  10-gonal numbers for which the sum of the digits is also a
 *          10-gonal number.
 * https://oeis.org/A119547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 2890, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth 10‑gonal number whose digital sum is also 10‑gonal
   (OEIS A119547).  The function enumerates decagonal numbers
   k*(4k-3) and keeps those with digit sum in
   {0,1,10,27,52,85,126}.  Results are memoised in a static array. */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* static memoisation state */
    static seq_t *cache = NULL;      /* array of found numbers          */
    static size_t cache_len = 0;     /* number of elements stored  */
    static size_t cache_cap = 0;     /* current allocation capacity   */
    static idx_t next_k = 0;         /* next k to process             */
    static seq_t cur_D = 0;          /* current decagonal value for k */

    /* generate until we have at least n values */
    while (cache_len < n) {
        /* Check if current decagonal number qualifies */
        seq_t s = digit_sum(cur_D);
        if (s == (seq_t)0 ||
            s == (seq_t)1 ||
            s == (seq_t)10 ||
            s == (seq_t)27 ||
            s == (seq_t)52 ||
            s == (seq_t)85 ||
            s == (seq_t)126) {

            /* ensure enough space */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* abort on bad alloc */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur_D;
        }

        /* Advance to next decagonal number: D_{k+1} = D_k + 8k + 1 */
        seq_t diff = (seq_t)8 * (seq_t)next_k + (seq_t)1;
        cur_D = cur_D + diff;
        next_k = next_k + 1;
    }

    return cache[n - 1];
}
