/*
 * A021364  Expansion of 1/((1-x)*(1-3*x)*(1-4*x)*(1-7*x)).
 * https://oeis.org/A021364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 75*a(n-2) + 145*a(n-3) - 84*a(n-4), a(0) = 1,
 *            a(1) = 15, a(2) = 150, a(3) = 1270
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the n-th term of OEIS sequence A021364 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* number of cached terms */

    /* Return cached value if already computed */
    if (n < cap)
        return cache[n];

    /* Grow cache to hold terms up to n inclusive */
    idx_t newcap = n + 1;
    size_t sz    = (size_t)newcap;

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * sz);
    } else if (newcap > cap) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * sz);
    }

    /* Initialize the first four terms if they are not yet set */
    if (cap == 0) {
        cache[0] = (seq_t)1;
        if (newcap > 1) cache[1] = (seq_t)15;
        if (newcap > 2) cache[2] = (seq_t)150;
        if (newcap > 3) cache[3] = (seq_t)1270;
    } else {
        if (cap <= 1 && newcap > 1) cache[1] = (seq_t)15;
        if (cap <= 2 && newcap > 2) cache[2] = (seq_t)150;
        if (cap <= 3 && newcap > 3) cache[3] = (seq_t)1270;
    }

    /* Compute missing terms using the linear recurrence */
    idx_t start = (cap < 4) ? 4 : cap;
    for (idx_t i = start; i < newcap; ++i) {
        seq_t t1 = (seq_t)15  * cache[i-1];
        seq_t t2 = (seq_t)75  * cache[i-2];
        seq_t t3 = (seq_t)145 * cache[i-3];
        seq_t t4 = (seq_t)84  * cache[i-4];

        seq_t val = t1;
        val -= t2;
        val += t3;
        val -= t4;

        cache[i] = val;
    }

    cap = newcap;
    return cache[n];
}
