/*
 * A091972  Expansion of g.f. (1 + x^5 ) / ( (1-x^3)*(1-x^4)).
 * https://oeis.org/A091972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)-a(n-2)+2*a(n-3)-a(n-4)+a(n-5)-a(n-6); * a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1 * Recurrence is valid for
 *            n>5. * The sequence is computed with memoisation; the
 *            implementation * works for any idx_t and seq_t defined by the
 *            build
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t allocated = 0;
    size_t sz = (size_t)n + 1;

    /* initialise base array if first call */
    if (!cache) {
        allocated = 6;           /* at least up to index 5 */
        cache = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure -> 0 */

        /* base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
    }

    /* enlarge cache if necessary */
    if (sz > allocated) {
        size_t new_alloc = sz;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute missing values up to n */
    size_t i = (size_t)cache[0]; /* we know cache size is at least 6 */
    /* Find current length (first missing element) */
    size_t cur_len = 0;
    while (cur_len < sz && cache[cur_len] != (seq_t)0) cur_len++;
    /* Since values are >=0, zeros cannot appear except uninitialised space;
       we will simply compute from cur_len (the first undefined index) */
    for (i = cur_len; i < sz; ++i) {
        if (i < 6) {  /* safety for indices <6 */
            /* already set above */
            continue;
        }
        seq_t t1 = cache[i - 1];
        t1 += cache[i - 3];
        t1 += cache[i - 3];   /* +2*a(i-3) */
        t1 += cache[i - 5];
        seq_t t2 = cache[i - 2];
        t2 += cache[i - 4];
        t2 += cache[i - 6];
        cache[i] = t1 - t2;
    }

    return cache[n];
}
