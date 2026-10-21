/*
 * A218003  Number of degree-n permutations of order a power of 3.
 * https://oeis.org/A218003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of n whose cycle lengths are
 *            powers of 3. Recurrence: A(0)=1; for n>0,
 *            A(n)=∑_{k∈{1,3,9,…,≤n}} ( (n-1)!/(n-k)! ) * A(n-k) where
 *            (n-1)!/(n-k)! = ∏_{t=n-k+1}^{n-1} t
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;   /* highest index computed so far */

    /* Initialise cache for n = 0 */
    if (cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;
        cap = 1;
    }

    /* Grow capacity if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    if ((idx_t)n > computed) {
        for (idx_t m = computed + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; k *= 3) {
                seq_t factor = 1;
                /* compute (m-1)!/(m-k)! as product t = m-k+1 .. m-1 */
                for (idx_t t = m - k + 1; t <= m - 1; ++t) {
                    factor *= (seq_t)t;
                }
                sum += factor * cache[m - k];
            }
            cache[m] = sum;
        }
        computed = n;
    }

    return cache[n];
}
