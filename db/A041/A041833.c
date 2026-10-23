/*
 * A041833  Denominators of continued fraction convergents to sqrt(437).
 * https://oeis.org/A041833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 9198*a(n-6) - a(n-12) for n>11 Initial values: a(0)=1,
 *            a(1)=1, a(2)=10, a(3)=21, a(4)=199, a(5)=220, a(6)=8999,
 *            a(7)=9219, a(8)=91970, a(9)=193159, a(10)=1830401, a(11)=2023560
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t capacity = 0;      /* allocated size of cache */
    static size_t last = (size_t)-1; /* largest index computed so far */

    /* Initialise cache and store the first 12 terms */
    if (cache == NULL) {
        /* ensure at least 12 slots */
        capacity = 12;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        /* first 12 terms */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)10;
        cache[3]  = (seq_t)21;
        cache[4]  = (seq_t)199;
        cache[5]  = (seq_t)220;
        cache[6]  = (seq_t)8999;
        cache[7]  = (seq_t)9219;
        cache[8]  = (seq_t)91970;
        cache[9]  = (seq_t)193159;
        cache[10] = (seq_t)1830401;
        cache[11] = (seq_t)2023560;
        last = 11;
    }

    /* Fast return for already computed term */
    if (n <= last)
        return cache[n];

    /* Resize cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = capacity;
        while ((size_t)n + 1 > newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        /* a(i) = 9198*a(i-6) - a(i-12)   (i≥12) */
        cache[i] = (seq_t)9198 * cache[i - 6] - cache[i - 12];
    }

    last = (size_t)n;
    return cache[n];
}
