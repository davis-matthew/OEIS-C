/*
 * A206630  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than four equal edges, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 66*a(n-1) + 970*a(n-2) + 9295*a(n-3) + 33266*a(n-4) +
 *            72056*a(n-5) - 224448*a(n-6) + 362688*a(n-7) - 1135552*a(n-8) +
 *            493824*a(n-9) - 311808*a(n-10) + 294912*a(n-11); Initial terms:
 *            a(1)=1085, a(2)=86392, a(3)=6885875, a(4)=548812835,
 *            a(5)=43740991174, a(6)=3486207195767, a(7)=277854703424...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n == 0)
        return (seq_t)0;  /* undefined for n=0 */

    /* initialize with the first 12 terms if not already done */
    if (cache == NULL) {
        cache_cap = 12;
        cache_len = 12;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0;   /* allocation failure */

        const char *init_str[12] = {
            "1085",
            "86392",
            "6885875",
            "548812835",
            "43740991174",
            "3486207195767",
            "277854703424333",
            "22145337882995800",
            "1765008776141025599",
            "140673219629487281855",
            "11211816614428114906158",
            "893594616847882973762891"
        };
        for (size_t i = 0; i < 12; ++i)
            cache[i] = seq_from_str(init_str[i]);
    }

    /* extend cache if n is larger than currently stored terms */
    if ((size_t)n > cache_len) {
        /* grow capacity */
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)n > new_cap)
            new_cap <<= 1;   /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* i is 0‑based index of a(i+1) to be computed */
            seq_t val = (seq_t)0;
            val += (seq_t)66      * cache[i-1];
            val += (seq_t)970     * cache[i-2];
            val += (seq_t)9295    * cache[i-3];
            val += (seq_t)33266   * cache[i-4];
            val += (seq_t)72056   * cache[i-5];
            val -= (seq_t)224448  * cache[i-6];
            val += (seq_t)362688  * cache[i-7];
            val -= (seq_t)1135552 * cache[i-8];
            val += (seq_t)493824  * cache[i-9];
            val -= (seq_t)311808  * cache[i-10];
            val += (seq_t)294912  * cache[i-11];
            cache[cache_len++] = val;
        }
    }

    return cache[n-1];
}
