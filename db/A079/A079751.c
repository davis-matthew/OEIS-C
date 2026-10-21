/*
 * A079751  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Algorithm L (lexicographic
 *          permutation generation) from Knuth's The Art of Computer
 *          Programming, Vol. 4, chapter 7.2.1.2. Sequence gives number of
 *          cases where the j search loop runs beyond j=n-3.
 * https://oeis.org/A079751
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * a(n‑1) + 1 for n >= 4, a(3) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* array of computed values */
static idx_t  a_alloc = 0;      /* allocated size of a_cache   */
static idx_t  a_last  = 0;      /* highest index that has been computed */

/* ensure that a_cache can hold indices up to n (inclusive) */
static void a_ensure(idx_t n)
{
    if (a_cache == NULL) {
        a_alloc = 8;
        a_cache = (seq_t*)malloc(a_alloc * sizeof(seq_t));
        if (!a_cache) exit(1);
        for (idx_t i = 0; i < a_alloc; ++i)
            a_cache[i] = (seq_t)0;
    }

    if (n + 1 <= a_alloc)
        return;

    idx_t new_alloc = a_alloc * 2;
    if (new_alloc < n + 2)
        new_alloc = n + 2;

    seq_t *tmp = (seq_t*)realloc(a_cache, new_alloc * sizeof(seq_t));
    if (!tmp) exit(1);

    for (idx_t i = a_alloc; i < new_alloc; ++i)
        tmp[i] = (seq_t)0;

    a_cache = tmp;
    a_alloc = new_alloc;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;                 /* outside of offset – return 0 */

    /* first call initialises everything */
    if (a_cache == NULL) {
        a_ensure(3);                     /* allocate at least up to index 3 */
        a_cache[3] = (seq_t)0;
        a_last = 3;
    }

    if (n <= a_last)
        return a_cache[n];

    a_ensure(n);                         /* make room for all needed indices */

    for (idx_t i = a_last + 1; i <= n; ++i) {
        if (i == 3)
            a_cache[i] = (seq_t)0;
        else
            a_cache[i] = (seq_t)i * a_cache[i-1] + 1;
    }

    a_last = n;
    return a_cache[n];
}
