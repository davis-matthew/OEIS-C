/*
 * A161159  a(n) = A003739(n)/(5*A001906(n)*A006238(n)).
 * https://oeis.org/A161159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 248*a(n-2) + 430*a(n-3) - 248*a(n-4) +
 *            40*a(n-5) - a(n-6) Initial values: a(1)=9, a(2)=245, a(3)=7776,
 *            a(4)=254035, a(5)=8336079, a(6)=273725760
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;   /* highest n that has been computed */

    /* ensure we can access n-1 index */
    if (cache == NULL || (size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        if (new_cap < 6) new_cap = 6;          /* ensure space for the first 6 terms */

        if (cache == NULL)
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

        cache_cap = new_cap;

        /* If this is the first allocation, initialise the first six terms */
        if (last_computed == 0) {
            cache[0] = (seq_t)9;
            cache[1] = (seq_t)245;
            cache[2] = (seq_t)7776;
            cache[3] = (seq_t)254035;
            cache[4] = (seq_t)8336079;
            cache[5] = (seq_t)273725760;
            last_computed = 6;
        }
    }

    /* If the requested n is already computed, return it */
    if (n <= last_computed)
        return cache[n-1];

    /* Compute terms from last_computed+1 up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i-2]; /* a(i-1) */
        seq_t a2 = cache[i-3]; /* a(i-2) */
        seq_t a3 = cache[i-4]; /* a(i-3) */
        seq_t a4 = cache[i-5]; /* a(i-4) */
        seq_t a5 = cache[i-6]; /* a(i-5) */
        seq_t a6 = cache[i-7]; /* a(i-6) */

        cache[i-1] = (seq_t)40 * a1
                   - (seq_t)248 * a2
                   + (seq_t)430 * a3
                   - (seq_t)248 * a4
                   + (seq_t)40 * a5
                   - a6;
    }

    last_computed = n;
    return cache[n-1];
}
