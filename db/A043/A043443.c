/*
 * A043443  Numbers having three 5's in base 8.
 * https://oeis.org/A043443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers having exactly three 5's in base 8, generation by
 *            brute‑force search with memoisation
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  The caller supplies the real types: seq_t and idx_t.  */
extern seq_t a(idx_t n)
{
    /* Memoised cache of already found terms.  */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cap   = 0;      /* allocated capacity   */
    static size_t len   = 0;      /* number of terms stored */
    static idx_t  start = 1;      /* next integer to test   */

    if (n == 0)
        return 0;                 /* undefined for offset 1 */

    /* Grow the cache until we have at least n terms.  */
    while (len < n) {
        /* Count the number of 5's in the base‑8 representation
           of the current integer.  */
        seq_t tmp = (seq_t)start;
        int    cnt = 0;
        while (tmp > (seq_t)0) {
            if ((tmp % (seq_t)8) == (seq_t)5)
                ++cnt;
            if (cnt > 3)          /* early exit if too many 5's */
                break;
            tmp /= (seq_t)8;
        }

        if (cnt == 3) {          /* keep the number if it matches */
            if (len == cap) {    /* need more space */
                size_t new_cap = cap ? cap * 2 : 16;
                /* realloc is used to enlarge the raw memory buffer.
                   C++ requires an explicit cast from void* to the desired
                   pointer type; the C style cast works in both C and
                   C++ and conforms to the restriction on unary operators. */
                seq_t *tmp_cache =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_cache)   /* allocation failure: give up */
                    return 0;
                cache = tmp_cache;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)start;
        }
        ++start;
    }

    return cache[n - 1];
}
