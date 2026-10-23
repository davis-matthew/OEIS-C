/*
 * A038151  Bilateral directed animals in first and 8th octants.
 * https://oeis.org/A038151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n-6)a(n‑1) + 2(n‑5)a(n‑2) – (11n‑41)a(n‑3) –
 *            (11n‑49)a(n‑4) – 3(n‑5)a(n‑5) divided by (n‑1). Initial values:
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=8, a(5)=23, a(6)=67
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (n < 1) return (seq_t)0;          /* undefined for n<1 – behaviour irrelevant */

    /* Resize cache if necessary */
    if (n + 1 > size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {            /* allocation failed – return 0 as a fallback */
            if (cache) free(cache);
            cache = NULL;
            size = 0;
            return (seq_t)0;
        }
        cache = tmp;
        if (size == 0) {
            cache[0] = (seq_t)0;   /* index 0 unused, keeps array 1‑based */
        }
        /* initialise newly added elements to 0 */
        for (idx_t i = size; i <= n; ++i) cache[i] = (seq_t)0;
        size = n + 1;
    }

    /* Base values – ensure they are set */
    if (n >= 1) cache[1] = (seq_t)1;
    if (n >= 2) cache[2] = (seq_t)1;
    if (n >= 3) cache[3] = (seq_t)3;
    if (n >= 4) cache[4] = (seq_t)8;
    if (n >= 5) cache[5] = (seq_t)23;
    if (n >= 6) cache[6] = (seq_t)67;

    /* Compute remaining values */
    for (idx_t i = 7; i <= n; ++i) {
        /* Coefficients are small enough to fit in idx_t */
        seq_t term1 = (seq_t)(4 * i - 6) * cache[i - 1];
        seq_t term2 = (seq_t)(2 * (i - 5)) * cache[i - 2];
        seq_t term3 = (seq_t)(11 * i - 41) * cache[i - 3];
        seq_t term4 = (seq_t)(11 * i - 49) * cache[i - 4];
        seq_t term5 = (seq_t)(3 * (i - 5)) * cache[i - 5];

        seq_t numerator = term1 + term2;
        numerator -= term3;
        numerator -= term4;
        numerator -= term5;

        cache[i] = numerator / (i - 1);
    }

    return cache[n];
}
