/*
 * A145136  Expansion of x/((1 - x - x^4)*(1 - x)^7).
 * https://oeis.org/A145136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(0)=0, a(1)=1, a(2)=8,
 *            a(3)=36, a(4)=120, a(5)=331, a(6)=801, a(7)=1761, a(8)=3597,
 *            a(9)=6931, a(10)=12737, and for n ≥ 11 a(n) = 8a(n-1) -28a(n-2)
 *            +56a(n-3) -69a(n-4) +49a(n-5) -7a(n-6) -27a(n-7) +34a(n-8)
 *            -21a(n-9) +7a(n-10) -a(n-11)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 43, u64 n <= 112, u128 n <= 250,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef uint64_t idx_t; /* index type used by the build system */

/* External linkage as required */
seq_t a(idx_t n) {
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated capacity */
    static size_t computed = 0;   /* highest computed index */

    /* Initialise cache on first call */
    if (cache == NULL) {
        size_t init = (n > 11 ? n + 1 : 12);   /* at least indices 0..10 */
        cache = (seq_t *)malloc(sizeof(seq_t) * init);
        cap   = init;

        /* Base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)36;
        cache[4] = (seq_t)120;
        cache[5] = (seq_t)331;
        cache[6] = (seq_t)801;
        cache[7] = (seq_t)1761;
        cache[8] = (seq_t)3597;
        cache[9] = (seq_t)6931;
        cache[10] = (seq_t)12737;

        computed = 10;
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= computed)
        return cache[n];

    /* Compute needed terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t v = (seq_t)0;

        /* Positive contributions */
        v += cache[i - 1] * (seq_t)8;
        v += cache[i - 3] * (seq_t)56;
        v += cache[i - 5] * (seq_t)49;
        v += cache[i - 8] * (seq_t)34;
        v += cache[i -10] * (seq_t)7;

        /* Negative contributions */
        v -= cache[i - 2] * (seq_t)28;
        v -= cache[i - 4] * (seq_t)69;
        v -= cache[i - 6] * (seq_t)7;
        v -= cache[i - 7] * (seq_t)27;
        v -= cache[i - 9] * (seq_t)21;
        v -= cache[i -11];

        cache[i] = v;
    }

    computed = n;
    return cache[n];
}
