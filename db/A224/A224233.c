/*
 * A224233  Decimal expansion of number of inches in a meter.
 * https://oeis.org/A224233
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-21) + a(n-22) for n > 23 Initial values (n =
 *            2 .. 24): a(2)=3, a(3)=9, a(4)=3, a(5)=7, a(6)=0, a(7)=0,
 *            a(8)=7, a(9)=8, a(10)=7, a(11)=4, a(12)=0, a(13)=1, a(14)=5,
 *            a(15)=7, a(16)=4, a(17)=8, a(18)=0, a(19)=3, a(20)=1, a(21)=4,
 *            a(22)=9, a(23)=6, a(24)=0
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t _init[23] = {
    3, 9, 3, 7, 0, 0, 7, 8, 7, 4, 0, 1, 5, 7, 4, 8, 0, 3, 1, 4, 9, 6, 0
};

seq_t a(idx_t n)
{
    /* a(0) and a(1) are undefined; return 0 for safety */
    if (n < 2) return 0;

    /* Static state for memoisation */
    static seq_t *cache = NULL;          /* array holding computed terms   */
    static unsigned char *filled = NULL; /* flags indicating whether term is set */
    static size_t cap = 0;               /* current capacity of cache      */

    /* Grow the cache and flag array if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp_cache) return 0;                        /* allocation failure */
        cache = tmp_cache;

        unsigned char *tmp_filled = (unsigned char *)realloc(filled, new_cap * sizeof(unsigned char));
        if (!tmp_filled) return 0;                        /* allocation failure */
        /* initialise newly allocated part to zero */
        if (!filled) {
            memset(tmp_filled, 0, new_cap);
        } else {
            memset(tmp_filled + cap, 0, new_cap - cap);
        }
        filled = tmp_filled;
        cap = new_cap;
    }

    /* Compute all terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        size_t ii = (size_t)i;
        if (filled[ii]) continue;          /* already computed */

        if (i <= 24) {                     /* use initial values */
            cache[ii] = _init[i - 2];
        } else {                            /* recurrence for i > 24 */
            long long v = (long long)cache[ii - 1]
                         - (long long)cache[ii - 21]
                         + (long long)cache[ii - 22];
            cache[ii] = (seq_t)v;           /* v will be 0..9 */
        }
        filled[ii] = 1;
    }

    return cache[n];
}
