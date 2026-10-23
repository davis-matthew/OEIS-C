/*
 * A116795  Number of permutations of length n which avoid the patterns 2341,
 *          3214, 4123.
 * https://oeis.org/A116795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 8*a(n-1) - 25*a(n-2)
 *            + 40*a(n-3) - 37*a(n-4) + 19*a(n-5) + 4*a(n-6) - 20*a(n-7) +
 *            18*a(n-8) - 7*a(n-9) + a(n-10) for n > 10, with initial values
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=70, a(6)=212, a(7)=611,
 *            a(8)=1712, a(9)=4712, a(10)=12815
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for 0 or negative */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;          /* highest index computed */

    /* initial allocation and base values */
    if (!cache) {
        /* allocate a little room for initial terms */
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)70;
        cache[6] = (seq_t)212;
        cache[7] = (seq_t)611;
        cache[8] = (seq_t)1712;
        cache[9] = (seq_t)4712;
        cache[10] = (seq_t)12815;
        cache_size = 10;
    }

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* compute using the recurrence only for i > 10 */
            seq_t pos = 0;
            seq_t sub = 0;

            /* positive contributions */
            pos += cache[(size_t)(i - 1)] * (seq_t)8;
            pos += cache[(size_t)(i - 3)] * (seq_t)40;
            pos += cache[(size_t)(i - 5)] * (seq_t)19;
            pos += cache[(size_t)(i - 6)] * (seq_t)4;
            pos += cache[(size_t)(i - 8)] * (seq_t)18;
            pos += cache[(size_t)(i -10)] * (seq_t)1;

            /* negative contributions */
            sub += cache[(size_t)(i - 2)] * (seq_t)25;
            sub += cache[(size_t)(i - 4)] * (seq_t)37;
            sub += cache[(size_t)(i - 7)] * (seq_t)20;
            sub += cache[(size_t)(i - 9)] * (seq_t)7;

            cache[(size_t)i] = pos - sub;
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}
