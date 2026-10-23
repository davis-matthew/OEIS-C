/*
 * A123015  Grow a binary tree using the following rules. Initially there is a
 *          single node labeled 1. At each step we add 1 to all labels less
 *          than 3. If a node has label 3 and zero or one descendants we add a
 *          new descendant labeled 1. Sequence gives sum of all labels at step
 *          n.
 * https://oeis.org/A123015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=4; for n>=4: a(n)=a(n-3)+a(n-4)+3
 *            (offset 0)
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size */
    static size_t computed   = 0;   /* number of indices that are initialized */

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1 > 4 ? n + 1 : 4);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();         /* out‑of‑memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise the first four terms if they are not yet set */
    if (computed < 4) {
        if (computed <= 0) cache[0] = (seq_t)1;
        if (computed <= 1) cache[1] = (seq_t)2;
        if (computed <= 2) cache[2] = (seq_t)3;
        if (computed <= 3) cache[3] = (seq_t)4;
        computed = 4;
    }

    /* compute missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 3] + cache[i - 4] + (seq_t)3;
    }
    computed = (size_t)n + 1;

    return cache[(size_t)n];
}
