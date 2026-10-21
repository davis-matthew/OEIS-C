/*
 * A229072  Lexicographically earliest sequence of distinct natural numbers
 *          such that, for any number n in the sequence, the positions of the
 *          1's in the binary representation of n are in the sequence, whereas
 *          the positions of the 0's are not.
 * https://oeis.org/A229072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1: a(n) = ∑_{a(i) ≤ n+1} 2^(n+1 – a(i)) The
 *            sum runs over all previous terms a(i)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a memoised list of terms so that
    arbitrary calls with any n≥1 will return the correct value
    in O(n log n) time and O(n) memory.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Handle invalid (pre‑offset) request calmly */
    if (n == (idx_t)0) return (seq_t)0;

    /* Static cache of computed terms, 1‑based in logic,
       stored 0‑based in array */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;       /* number of terms stored */

    /* Allocate the first term if cache is empty */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely */
        cache[0] = (seq_t)1;          /* a(1) = 1 */
    }

    /* Grow the cache to contain all terms up to n */
    if (n > (idx_t)cache_len) {
        idx_t new_len = cache_len * 2;
        if (new_len < n) new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failure – unlikely */
        cache = tmp;
        /* Compute missing terms sequentially */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /* Sum over all previous terms that are ≤ i+1 */
            for (idx_t j = 1; j < i; ++j) {
                /* cache[j-1] is a(j) */
                if (cache[j-1] <= (seq_t)(i + 1)) {
                    /* Exponent is (i+1 - a(j)) and is ≤ i+1 because
                       a(j) ≤ i+1, hence safe to cast to idx_t.  */
                    idx_t exp = (idx_t)((i + 1) - (seq_t)cache[j-1]);
                    sum += pow2(exp);
                }
            }
            cache[i - 1] = sum;   /* store a(i) */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
