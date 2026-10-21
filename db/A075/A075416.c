/*
 * A075416  Squares of A002281.
 * https://oeis.org/A075416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3); Basing values on
 *            a(0)=0, a(1)=49, a(2)=5929 (This is equivalent to a(n) =
 *            49*(10^n-1)^2/81)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;   /* number of terms currently stored */

    /* Ensure enough space for term n */
    if (n >= cap) {
        size_t newcap = cap ? cap : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: abort silently */
            return 0;
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Fill in missing terms up to n */
    while (len <= n) {
        if (len == 0) {              /* a(0) */
            cache[0] = 0;
            len = 1;
        } else if (len == 1) {       /* a(1) */
            cache[1] = 49;
            len = 2;
        } else if (len == 2) {       /* a(2) */
            cache[2] = 5929;
            len = 3;
        } else {                     /* recurrence for len >= 3 */
            seq_t v = 111 * cache[len - 1];
            v -= 1110 * cache[len - 2];
            v += 1000 * cache[len - 3];
            cache[len] = v;
            len++;
        }
    }

    return cache[n];
}
