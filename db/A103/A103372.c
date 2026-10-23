/*
 * A103372  a(1) = a(2) = a(3) = a(4) = a(5) = 1 and for n>5: a(n) = a(n-4) +
 *          a(n-5).
 * https://oeis.org/A103372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5), a(1) = a(2) = a(3) = a(4) = a(5) = 1
 * Range:     u8 n <= 38, u16 n <= 74, u32 n <= 146, u64 n <= 289,
 *            u128 n <= 576, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised results */
    static seq_t *cache = NULL;   /* array indices 0 … computed (0 unused) */
    static idx_t allocated = 0;   /* number of slots currently allocated */
    static idx_t computed  = 0;   /* highest index already computed */

    /* first call: initialise cache */
    if (cache == NULL) {
        allocated = 6;                 /* indices 0 … 5 */
        /* malloc returns void*, cast explicitly to seq_t*             */
        cache = (seq_t*)malloc(allocated * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory – unlikely in tests */

        cache[0] = (seq_t)0;          /* unused */
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = (seq_t)1;
        computed  = 5;
    }

    /* if already known, return it */
    if (n <= computed)
        return cache[n];

    /* grow the array if needed */
    if (n + 1 > allocated) {
        idx_t new_alloc = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
        if (tmp == NULL) return 0;    /* allocation failure – unlikely */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i)
        cache[i] = cache[i - 4] + cache[i - 5];

    computed = n;
    return cache[n];
}
