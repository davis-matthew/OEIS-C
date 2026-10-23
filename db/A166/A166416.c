/*
 * A166416  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166416 (OEIS)
 *
 * Formula: a(0)=1, a(1)=22, a(2)=462, a(3)=9702, a(4)=203742,
 * a(5)=4278582, a(6)=89850222, a(7)=1886854662, a(8)=39623947902,
 * a(9)=832102905942, a(10)=17474161024782, a(11)=366957381520191,
 * a(n) = 20 * (a(n-1)+...+a(n-10)) - 210 * a(n-11)   for n ≥ 12
 *
 * This implementation memoises previously computed values in a
 * dynamically‑resizable array.  All arithmetic is performed in the
 * type seq_t, ensuring exactness for any supported precision.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Grow a dynamic array to hold at least `needed` elements.  The
 * array is stored in the pointer `array` and its current capacity is
 * held in *cap.  This helper works for any element size. */
static void ensure_capacity(size_t *cap, seq_t **array, size_t needed)
{
    if (*cap < needed)
    {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        *array = tmp;
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* pointer to array of terms */
    static size_t cap     = 0;       /* current capacity of cache */
    static idx_t len      = 0;       /* number of terms computed   */
    static bool   init    = false;   /* have the initial 12 terms been stored? */

    /* if the cache hasn't been initialised or we need terms beyond what
     * we have, enlarge it and compute missing values. */
    if (!init || len <= n)
    {
        /* we always need at least 12 terms, because the recurrence
         * accesses a(n-11).  For requested n < 12 we simply allocate
         * space for 12 entries. */
        size_t required = (n >= 12) ? (size_t)n + 1 : 12;
        ensure_capacity(&cap, &cache, required);

        if (!init)
        {
            /* initialise the first twelve terms (base case).  These
             * constants come directly from the definition of the
             * sequence; they are required seeds for the recurrence. */
            cache[0]  = (seq_t)1ULL;
            cache[1]  = (seq_t)22ULL;
            cache[2]  = (seq_t)462ULL;
            cache[3]  = (seq_t)9702ULL;
            cache[4]  = (seq_t)203742ULL;
            cache[5]  = (seq_t)4278582ULL;
            cache[6]  = (seq_t)89850222ULL;
            cache[7]  = (seq_t)1886854662ULL;
            cache[8]  = (seq_t)39623947902ULL;
            cache[9]  = (seq_t)832102905942ULL;
            cache[10] = (seq_t)17474161024782ULL;
            cache[11] = (seq_t)366957381520191ULL;
            len      = 12;
            init     = true;
        }

        /* compute terms up to the requested index n */
        while (len <= n)
        {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 10; ++j)
                sum += cache[len - j];
            seq_t val = 20 * sum - (seq_t)210 * cache[len - 11];
            cache[len] = val;
            ++len;
        }
    }

    return cache[n];
}
