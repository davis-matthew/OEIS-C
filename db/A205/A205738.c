/*
 * A205738  Number of 3X(n+1) 0..2 arrays with every 2 X 2 subblock having the
 *          same number of clockwise edge increases as its horizontal
 *          neighbors and no 2 X 2 subblock having the same number of
 *          counterclockwise edge increases as its vertical neighbors.
 * https://oeis.org/A205738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n>13 follows the empirical recurrence a(n) = 6*a(n-1)
 *            -2*a(n-2) -21*a(n-3) +a(n-4) +25*a(n-5) +14*a(n-6) -4*a(n-8)
 *            -2*a(n-9) Initial values (n=1..13): 360, 978, 3888, 17532,
 *            80916, 376314, 1756446, 8212662, 38432652, 179925948, 842504430,
 *            3945403368, 18476947170 The implementation stores...
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[] = {
    (seq_t)360,
    (seq_t)978,
    (seq_t)3888,
    (seq_t)17532,
    (seq_t)80916,
    (seq_t)376314,
    (seq_t)1756446,
    (seq_t)8212662,
    (seq_t)38432652,
    (seq_t)179925948,
    (seq_t)842504430,
    (seq_t)3945403368,
    (seq_t)18476947170
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for 0, return 0 as safe */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Initial cache setup */
    if (!cache) {
        cap = sizeof(init_vals)/sizeof(init_vals[0]); /* 13 */
        cache = seq_alloc(cap);
        for (size_t i = 0; i < cap; ++i) cache[i] = init_vals[i];
        len = cap;
    }

    /* Ensure capacity for n terms */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) return (seq_t)0; /* memory failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using recurrence */
    while (len < (size_t)n) {
        size_t idx = len; /* 0‑based index corresponding to a(idx+1) */
        seq_t val;
        if (idx < 13) {
            val = init_vals[idx];
        } else {
            /* recurrence: a[n] = 6*a[n-1] -2*a[n-2] -21*a[n-3] +a[n-4]
                         +25*a[n-5] +14*a[n-6] -4*a[n-8] -2*a[n-9] */
            val  = (seq_t)6  * cache[idx-1];
            val -= (seq_t)2  * cache[idx-2];
            val -= (seq_t)21 * cache[idx-3];
            val +=            cache[idx-4];
            val += (seq_t)25 * cache[idx-5];
            val += (seq_t)14 * cache[idx-6];
            val -= (seq_t)4  * cache[idx-8];
            val -= (seq_t)2  * cache[idx-9];
        }
        cache[idx] = val;
        ++len;
    }

    return cache[(size_t)n - 1];
}
