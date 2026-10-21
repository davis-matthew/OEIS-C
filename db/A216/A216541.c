/*
 * A216541  Product of Lucas and Catalan numbers: a(n) =
 *          A000032(n+1)*A000108(n).
 * https://oeis.org/A216541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas(n+1) * Catalan(n) Lucas(0)=2, Lucas(1)=1,
 *            Lucas(k)=Lucas(k-1)+Lucas(k-2) (k≥2) Catalan(n) is obtained from
 *            the utility routine catalan()
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* static cache for Lucas numbers: indices 0… */
    static seq_t *lucas_cache = NULL;
    static size_t lucas_len = 0;     /* elements currently valid   */
    static size_t lucas_cap = 0;     /* allocated capacity         */

    /* ensure cache contains Lucas(n+1) at index n+1 */
    if (lucas_len <= (size_t)(n + 1)) {
        size_t needed = (size_t)(n + 2);   /* indices 0…n+1 inclusive */

        /* enlarge storage if necessary */
        if (lucas_cap < needed) {
            size_t new_cap = lucas_cap ? lucas_cap : 4;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = NULL;
            if (lucas_cache == NULL)
                tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            else
                tmp = (seq_t*)realloc(lucas_cache, new_cap * sizeof(seq_t));
            lucas_cache = tmp;
            lucas_cap  = new_cap;
        }

        /* initialise new elements */
        if (lucas_len == 0) {
            lucas_cache[0] = (seq_t)2;
            lucas_len = 1;
        }
        if (lucas_len == 1) {
            lucas_cache[1] = (seq_t)1;
            lucas_len = 2;
        }
        for (size_t k = lucas_len; k <= (size_t)n + 1; ++k) {
            lucas_cache[k] = lucas_cache[k-1] + lucas_cache[k-2];
        }
        lucas_len = needed;
    }

    seq_t lucas_val = lucas_cache[(size_t)n + 1];
    seq_t cat_val   = catalan(n);
    return lucas_val * cat_val;
}
