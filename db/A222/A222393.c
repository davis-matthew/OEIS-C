/*
 * A222393  Nonnegative integers m such that 18*m*(m+1)+1 is a square.
 * https://oeis.org/A222393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + 34*a(n‑2) – 34*a(n‑3) – a(n‑4) + a(n‑5) * with
 *            initial values: a(1)=0, a(2)=4, a(3)=12, a(4)=152, a(5)=424
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached values of the sequence */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (number of elements)  */
    static idx_t len    = 0;   /* largest index that has been computed   */

    /* Initial terms (index 0 unused) */
    static const seq_t init[6] = {0, 0, 4, 12, 152, 424};

    /* For n <= 5 we can answer directly */
    if (n <= 5) return init[n];

    /* Initialise cache on first request of a value > 5 */
    if (cache == NULL) {
        /* Ensure a modest initial capacity (growable later) */
        size_t initcap = (size_t)(n > 32 ? n * 2 : 32);
        cache = (seq_t *)malloc(initcap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed – unlikely in tests */
        cap   = initcap;
        /* Store the first five terms */
        for (idx_t i = 1; i <= 5; ++i) cache[i] = init[i];
        len = 5;
    }

    /* Grow cache if necessary.  New capacity doubles until it fits n. */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1; /* multiply by 2 */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – unlikely in tests */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n */
    if (n > len) {
        for (idx_t i = len + 1; i <= n; ++i) {
            cache[i] = cache[i - 1]
                     + 34 * cache[i - 2]
                     - 34 * cache[i - 3]
                     - cache[i - 4]
                     + cache[i - 5];
        }
        len = n;
    }

    return cache[n];
}
