/*
 * A125050  a(1) = 1. If a(n) is composite, a(n+1) = 2*a(n)+1; otherwise,
 *          a(n+1) = 2*a(n).
 * https://oeis.org/A125050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = 2*a(n)+1 if a(n) is composite, otherwise
 *            a(n+1) = 2*a(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 21, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/seq_alloc_array.h"/* seq_t *seq_alloc_array(size_t) */
#include "../utils/seq_free_array.h"  /* void seq_free_array(seq_t*) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;          /* array of already computed terms */
    static size_t cap     = 0;             /* allocated capacity */
    static idx_t  len     = 0;             /* number of terms stored */

    /* grow cache if needed */
    if ((size_t)len < n) {
        if (cap < (size_t)n) {             /* increase capacity */
            size_t newcap = cap ? cap : 8;
            while (newcap < (size_t)n) newcap <<= 1;
            seq_t *tmp = seq_alloc_array(newcap);
            for (size_t i = 0; i < (size_t)len; ++i) tmp[i] = cache[i];
            if (cache) seq_free_array(cache);
            cache = tmp;
            cap   = newcap;
        }

        /* initialize first term if cache empty */
        if (len == 0) {
            cache[0] = (seq_t)1;
            len = 1;
        }

        /* generate missing terms up to n */
        for (idx_t i = len; i < n; ++i) {
            seq_t prev = cache[i - 1];
            /* a(n) is composite iff prev > 1 && not prime */
            bool composite = (prev > (seq_t)1 && !is_prime_big(prev));
            cache[i] = prev * (seq_t)2 + (composite ? (seq_t)1 : (seq_t)0);
        }
        len = n;
    }

    return cache[n - 1];
}
