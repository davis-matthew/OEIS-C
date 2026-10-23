/*
 * A191864  a(n) = (a(n-1) + a(n-4)) * (a(n-2) - a(n-3)) with a(1)=1, a(2)=2,
 *          a(3)=3 and a(4)=4.
 * https://oeis.org/A191864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-4)) * (a(n-2)-a(n-3)) * with a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=4, n≥1 * This implementation memoizes computed
 *            terms in a static array. * It works for any idx_t/seq_t defined
 *            by the build environment * (e.g. unsigned integers or
 *            arbitrary‑precision integers)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 12, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* behaviour for n<1 not required   */

    /* static cache of computed values */
    static seq_t *cache = NULL;          /* array indexed by n (1‑based)   */
    static idx_t cap = 0;                /* largest index stored           */
    static idx_t alloc_size = 0;         /* allocated array size (bytes)    */
    static int initialized = 0;

    /* first time: allocate space for indices 0..4 and set base values */
    if (!initialized) {
        alloc_size = 5;                  /* need indices 0..4 inclusive   */
        cache = (seq_t *)malloc(sizeof(seq_t) * alloc_size);
        if (!cache) return (seq_t)0;     /* allocation failure (unlikely) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cap = 4;
        initialized = 1;
    }

    /* if requested index already computed, return it */
    if (n <= cap) return cache[n];

    /* enlarge cache if necessary (retain existing values) */
    if (n >= alloc_size) {
        idx_t new_size = n + 1;          /* need space up to index n    */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;       /* allocation failure            */
        cache = tmp;
        alloc_size = new_size;
    }

    /* compute missing terms up to n sequentially */
    for (idx_t i = cap + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] + cache[i - 4];
        seq_t t2 = cache[i - 2] - cache[i - 3];
        cache[i] = t1 * t2;
    }
    cap = n;
    return cache[n];
}
