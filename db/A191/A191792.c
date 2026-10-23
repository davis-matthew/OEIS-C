/*
 * A191792  Number of length n left factors of Dyck paths having no UDUD's;
 *          here U=(1,1) and D=(1,-1).
 * https://oeis.org/A191792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191792 – Number of left factors of Dyck paths of length n having no UDUD pattern
   Recurrence (conjectured order 8):
      (n+1)*a(n) = 2*a(n-1)+2*(n-1)*a(n-2)+4*a(n-3)+5*(n-3)*a(n-4)+2*(n-5)*a(n-6)
                    -4*a(n-5)-2*a(n-7)-(n-7)*a(n-8)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=15, a(7)=25.
*/

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build. */

/* Computes a(n) as defined for sequence A191792. */
seq_t a(idx_t n)
{
    /* Integer arithmetic with seq_t; all casts from idx_t ensure
       the factor (k) is treated as seq_t before multiplication.    */
    static const seq_t init_vals[8] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                       (seq_t)3, (seq_t)5, (seq_t)8,
                                       (seq_t)15, (seq_t)25 };

    /* Cache of computed values; grows automatically. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries allocated */
    static size_t computed = 0;    /* number of entries actually filled */

    /* Allocate or resize the cache if needed. */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort as per build policy */
            abort();
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* Initialise first 8 values if not already done. */
    if (computed == 0) {
        size_t init_len = (n < 8 ? (size_t)n + 1 : 8);
        for (size_t i = 0; i < init_len; ++i) {
            cache[i] = init_vals[i];
        }
        computed = init_len;
    }

    /* Compute missing terms up to n. */
    for (size_t k = computed; k <= (size_t)n; ++k) {
        if (k < 8) {
            cache[k] = init_vals[k];
            continue;
        }
        /* Positive contributions to the numerator. */
        seq_t pos = ((seq_t)2) * cache[k-1];
        pos += ((seq_t)2) * ((seq_t)(k-1)) * cache[k-2];
        pos += ((seq_t)4) * cache[k-3];
        pos += ((seq_t)5) * ((seq_t)(k-3)) * cache[k-4];
        pos += ((seq_t)2) * ((seq_t)(k-5)) * cache[k-6];

        /* Negative contributions. */
        seq_t neg = ((seq_t)4) * cache[k-5];
        neg += ((seq_t)2) * cache[k-7];
        neg += ((seq_t)(k-7)) * cache[k-8];

        /* Numerator is positive; use unsigned subtraction. */
        seq_t num = pos - neg;
        cache[k] = num / ( (seq_t)(k + 1) );
    }

    computed = (size_t)n + 1;
    return cache[n];
}
