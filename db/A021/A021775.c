/*
 * A021775  Decimal expansion of 1/771.
 * https://oeis.org/A021775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimals of 1/771:
   a(n) = the n‑th digit after the decimal point in the decimal expansion of 1/771. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent state for the generated digits. */
    static seq_t *cache = NULL;   /* array of digits stored */
    static size_t cap    = 0;     /* allocated capacity of cache   */
    static idx_t  len    = 0;     /* number of digits stored       */
    static seq_t rem     = 1;     /* current remainder before next digit */

    const unsigned denom = 771;

    /* Extend the cache if we have not yet generated up to digit n. */
    if (len <= n) {
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            size_t newcap = cap ? cap << 1 : 8;
            if (newcap < needed) newcap = needed;
            seq_t *tmp;
#ifdef __cplusplus
            /* Allocate a new block, copy existing elements, and free the old block. */
            tmp = new seq_t[newcap];
            for (idx_t i = 0; i < len; ++i) {
                tmp[i] = cache[i];
            }
            if (cache) delete [] cache;
#else
            /* For plain C, use realloc which copies the raw memory. */
            tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed */
#endif
            cache = tmp;
            cap   = newcap;
        }

        /* Generate the remaining digits up to n. */
        for (idx_t i = len; i <= n; ++i) {
            rem *= 10;
            seq_t digit = rem / denom;
            rem = rem % denom;
            cache[i] = digit;
        }
        len = n + 1;
    }
    return cache[n];
}
