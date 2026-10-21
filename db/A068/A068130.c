/*
 * A068130  Triangular numbers with sum of digits = 15.
 * https://oeis.org/A068130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th triangular number whose decimal digit sum equals 15
   by searching triangular numbers in order. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* the sequence is indexed from 1; a(0) is undefined – return 0 */
    if (n == 0) return 0;

    /* cache of the found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored   */
    static idx_t cache_cap = 0;   /* allocated capacity of the cache     */

    /* state for continuing the search */
    static idx_t next_k = 1;      /* next index k for T_k                */
    static seq_t next_tri = 0;    /* T_{next_k-1}                       */

    /* Make sure we have at least n terms in the cache */
    if (n > cache_len) {
        /* enlarge the cache if necessary */
        if (cache_cap < n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* generate triangular numbers until we have n terms */
        while (cache_len < n) {
            /* T_k = T_{k-1} + k */
            next_tri += (seq_t)next_k;

            /* a quick filter: digit sum 15 implies T_k ≡ 6 (mod 9) */
            if ((next_tri % 9) == 6) {
                seq_t tmp = next_tri;
                seq_t sum = 0;
                while (tmp) {
                    sum += tmp % 10;
                    tmp /= 10;
                }
                if (sum == 15) {
                    cache[cache_len++] = next_tri;
                }
            }
            ++next_k;   /* advance to the next k */
        }
    }

    return cache[n - 1];
}
