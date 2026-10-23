/*
 * A119477  a(1)=1, a(n) = a((n+1)/2) + 2 if n is odd, a(n) = a(n/2) + 1 if n
 *          is even.
 * https://oeis.org/A119477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = a((n+1)/2) + 2 if n is odd a(n) = a(n/2) + 1 if
 *            n is even
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap    = 0;

    if (n == 0) return (seq_t)0;          /* undefined, not used */

    /* Ensure cache can hold indices up to n */
    size_t needed = (size_t)n + 1;        /* indices start at 1 */
    if (needed > cap) {
        seq_t *tmp = (cap == 0) ? (seq_t*)malloc(needed * sizeof(seq_t))
                                : (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        /* initialize newly allocated part to 0 */
        if (cap < needed)
            memset(tmp + cap, 0, (needed - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = needed;
    }

    /* base case */
    if (n == 1) {
        cache[1] = (seq_t)1;
        return cache[1];
    }

    /* return cached value if present */
    if (cache[n] != (seq_t)0) return cache[n];

    seq_t res;
    if ((n & 1ULL) == 0) {                /* even */
        res = a(n / 2);
        res += (seq_t)1;
    } else {                              /* odd */
        res = a((n + 1) / 2);
        res += (seq_t)2;
    }

    cache[n] = res;
    return res;
}
