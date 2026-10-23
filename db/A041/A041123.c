/*
 * A041123  Denominators of continued fraction convergents to sqrt(70).
 * https://oeis.org/A041123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 502*a(n‑6) - a(n‑12) with initial values: a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=8, a(4)=11, a(5)=30, a(6)=491, a(7)=1012,
 *            a(8)=1503, a(9)=4018, a(10)=5521, a(11)=15060
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 12 terms */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)2,  (seq_t)3,
        (seq_t)8,  (seq_t)11, (seq_t)30,
        (seq_t)491,(seq_t)1012,(seq_t)1503,
        (seq_t)4018,(seq_t)5521,(seq_t)15060
    };

    /* dynamic cache */
    static seq_t *cache = NULL;
    static size_t  size   = 0;    /* number of values stored */
    static size_t  capacity = 0;  /* allocated size */

    size_t i;

    /* initial allocation of the first 12 terms */
    if (size == 0) {
        capacity = 12;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        for (i = 0; i < 12; ++i) {
            cache[i] = init[i];
        }
        size = 12;
    }

    /* if the requested index is already computed, return it */
    if (n < size) {
        return cache[n];
    }

    /* grow the cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute new values using the recurrence */
    for (i = size; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)502 * cache[i - 6] - cache[i - 12];
    }

    size = (size_t)n + 1;
    return cache[n];
}
