/*
 * A017829  Expansion of 1/(1 - x^4 - x^5 - x^6 - x^7).
 * https://oeis.org/A017829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + a(n-5) + a(n-6) + a(n-7) Initial values: a(0)=1,
 *            a(1)=a(2)=a(3)=0, a(4)=a(5)=a(6)=a(7)=1
 * Range:     u8 n <= 27, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, free */

seq_t a(idx_t n) {
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;       /* allocated size of cache   */
    static size_t next  = 0;       /* first index not yet computed (i.e. cache[0..next-1] valid) */

    /* Ensure the cache is large enough */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 32;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed, return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize base case values on first use */
    if (next == 0) {
        cache[0] = (seq_t)1;
        for (size_t i = 1; i <= 7; ++i)               /* a(1)..a(3) = 0, a(4)..a(7) = 1 */
            cache[i] = (seq_t)0;
        cache[4] = cache[5] = cache[6] = cache[7] = (seq_t)1;
        next = 8;  /* first index not yet computed */
    }

    /* Compute up to n if necessary */
    if (next <= (size_t)n) {
        /* recurrence holds for indices >= 8 */
        for (size_t i = next; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 4] + cache[i - 5] + cache[i - 6] + cache[i - 7];
        }
        next = (size_t)n + 1;  /* all indices < next are now computed */
    }

    return cache[(size_t)n];
}
