/*
 * A136779  Number of multiplex juggling sequences of length n, base state
 *          <1,1,1> and hand capacity 2.
 * https://oeis.org/A136779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 13*a(n-2) for n>3 * Initial values: a(1)=1,
 *            a(2)=3, a(3)=18
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_N 20000 /* safe upper bound for this project */

static seq_t cache[MAX_N];
static idx_t cache_count = 0;

seq_t a(idx_t n)
{
    /* offset is 1; values for n<1 are undefined */
    if (n == 0) return (seq_t)0;

    /* lazy initialization of the cache */
    if (cache_count == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)18;
        cache_count = 3;
    }

    /* return already computed value if possible */
    if (n <= cache_count) {
        return cache[n-1];
    }

    /* safeguard against overflow of the static array */
    assert(n <= MAX_N);

    /* extend the cache up to index n */
    for (idx_t i = cache_count + 1; i <= n; ++i) {
        seq_t v1 = cache[i-2]; /* a(i-1) */
        seq_t v2 = cache[i-3]; /* a(i-2) */
        /* recurrence: a(i) = 8*a(i-1) - 13*a(i-2) */
        seq_t val = (seq_t)8 * v1 - (seq_t)13 * v2;
        cache[i-1] = val;
    }
    cache_count = n;

    return cache[n-1];
}
