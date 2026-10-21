/*
 * A074081  Sum of determinants of 3rd-order principal minors of powers of
 *          inverse of tetramatrix ((1,1,0,0),(1,0,1,0),(1,0,0,1),(1,0,0,0)).
 * https://oeis.org/A074081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1)+a(n-2)-a(n-3)+a(n-4), a(0)=4, a(1)=-1, a(2)=3,
 *            a(3)=-7
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;

/* ensure the cache has room for at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap > 0 ? cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* memory exhausted */
    cache = tmp;
    cap   = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise first four terms if not yet done */
    if (len == 0) {
        ensure_capacity(4);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)-7;
        len = 4;
    }

    size_t sn = (size_t)n;
    if (sn >= len) {
        /* grow cache to include up to n */
        ensure_capacity(sn + 1);
        for (size_t i = len; i <= sn; ++i) {
            seq_t val = -cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4];
            cache[i] = val;
        }
        len = sn + 1;
    }
    return cache[sn];
}
