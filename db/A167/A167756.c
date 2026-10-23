/*
 * A167756  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1,  a(n) = 29*a(n-1)+29*a(n-2)+...+29*a(n-14)-435*a(n-15) for n>=16 */
/* a(15) = 29*a(14)+...+29*a(1)-435*a(0)+1 */
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array */
#include "../utils/seq_free_array.h"    /* seq_free_array */
#include "../utils/seq_from_str.h"     /* seq_from_str */

seq_t a(idx_t n)
{
    /* cache of computed values */
    static seq_t *cache   = NULL;   /* a(0)..a(cache_len-1) */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_len = 0;    /* number of terms stored */

    /* initialise cache with first 15 known terms once */
    if (cache_len == 0) {
        size_t init_cap = 16;               /* at least 16 slots */
        cache = seq_alloc_array(init_cap);
        cache_cap = init_cap;

        /* first 15 terms (indices 0..14) */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)31;
        cache[2]  = (seq_t)930;
        cache[3]  = (seq_t)27900;
        cache[4]  = (seq_t)837000;
        cache[5]  = (seq_t)25110000;
        cache[6]  = (seq_t)753300000;
        cache[7]  = (seq_t)22599000000ULL;
        cache[8]  = (seq_t)677970000000ULL;
        cache[9]  = (seq_t)20339100000000ULL;
        cache[10] = (seq_t)610173000000000ULL;
        cache[11] = (seq_t)18305190000000000ULL;
        cache[12] = (seq_t)549155700000000000ULL;
        cache[13] = (seq_t)16474671000000000000ULL;
        cache[14] = seq_from_str("494240130000000000000");
        cache_len = 15;   /* indices 0..14 have values */
    }

    /* if the requested index is already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        /* copy existing values */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute remaining terms up to n using the recurrence */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i <= 14; ++i)
            sum += cache[k - i] * (seq_t)29;
        sum -= cache[k - 15] * (seq_t)435;
        if (k == 15)            /* account for the +1 from the numerator */
            sum += (seq_t)1;
        cache[k] = sum;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
