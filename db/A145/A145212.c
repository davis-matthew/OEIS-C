/*
 * A145212  Numbers x such that there exists n in N : (x+1)^3 - x^3 = 67*n^2.
 * https://oeis.org/A145212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 1030190*a(n+1) - a(n) + 515094 a(1) = 146 a(2) =
 *            150922981
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small constants used in the recurrence */
    const seq_t A1 = 146;
    const seq_t A2 = 150922981;
    const seq_t COEF = 1030190;
    const seq_t CCONST = 515094;

    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;      /* number of terms stored in cache */
    static idx_t capacity = 0;  /* current allocated capacity */

    /* Immediate return for the first two terms */
    if (n == 1) return A1;
    if (n == 2) return A2;

    /* Initialise cache on first call */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory fallback */
        cache[0] = A1;
        cache[1] = A2;
        size = 2;
    }

    /* Grow cache if necessary */
    if (n > size) {
        if (capacity < n) {
            idx_t newcap = capacity * 2;
            if (newcap < n) newcap = n;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) { return 0; }   /* out of memory fallback */
            cache = tmp;
            capacity = newcap;
        }
        /* Compute missing terms using the recurrence */
        for (idx_t i = size + 1; i <= n; ++i) {
            /* a(i) = COEF * a(i-1) - a(i-2) + CCONST */
            seq_t prev      = cache[i - 2];   /* a(i-1) */
            seq_t prevprev  = cache[i - 3];   /* a(i-2) */
            cache[i - 1] = COEF * prev - prevprev + CCONST;
        }
        size = n;
    }

    return cache[n - 1];
}
