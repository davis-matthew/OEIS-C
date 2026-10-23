/*
 * A116509  Values of c in a^2 + b^2 = c^2 where b - a = 31 and gcd(a,b)=1.
 * https://oeis.org/A116509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   c(n) = 6*c(n-2) - c(n-4), c(1)=41, c(2)=109, c(3)=221, c(4)=629
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A116509 : Values of c in a^2 + b^2 = c^2 with b - a = 31 and gcd(a,b)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but guard */

    /* static cache of computed terms (1‑indexed in terms of the user) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms already stored */

    /* ensure the cache holds at least n terms (or 4 for base values) */
    if (n > cached) {
        /* determine capacity: at least 4 for the base, otherwise n */
        idx_t new_cap = (n > 4) ? n : 4;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }

        /* initialise the base values if this is the first allocation */
        if (cached == 0) {
            cache[0] = 41;
            cache[1] = 109;
            cache[2] = 221;
            cache[3] = 629;
            cached = 4;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = cached; i < new_cap; ++i) {
            /* i is zero‑based; term number is i+1 */
            if (i < 4) continue;   /* safeguard; should not happen */
            cache[i] = (seq_t)6 * cache[i - 2] - cache[i - 4];
        }
        cached = new_cap;
    }

    return cache[n - 1];
}
