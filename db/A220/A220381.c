/*
 * A220381  Majority value maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal and antidiagonal neighbors in a random 0..1 n X 3
 *          array.
 * https://oeis.org/A220381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2) + 3*a(n-3) - 3*a(n-4) - 2*a(n-5)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n ≥ 6.  Initial values: a(1)=4, a(2)=25, a(3)=136, a(4)=753, a(5)=4160 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t computed = 0;          /* number of terms currently stored */

    /* initialise cache with the first five terms once */
    if (computed == 0) {
        size_t init = 5;
        cap = init;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;             /* allocation failed */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)136;
        cache[3] = (seq_t)753;
        cache[4] = (seq_t)4160;
        computed = init;
    }

    /* return already computed value if available */
    if (n <= computed)
        return cache[n-1];

    /* enlarge storage if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;             /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms from 'computed' up to n */
    for (idx_t i = computed; i < n; ++i) {
        idx_t idx = i;                    /* zero‑based index for a(i+1) */
        seq_t val = (seq_t)4 * cache[idx-1];
        val += (seq_t)8 * cache[idx-2];
        val += (seq_t)3 * cache[idx-3];
        val -= (seq_t)3 * cache[idx-4];
        val -= (seq_t)2 * cache[idx-5];
        cache[idx] = val;
    }

    computed = n;
    return cache[n-1];
}
