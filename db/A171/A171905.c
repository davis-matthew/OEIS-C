/*
 * A171905  a(1) = 1 and a(2) = 2, a(n) = |(sum of previous terms) - n|.
 * https://oeis.org/A171905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n) = |(sum_{i=1}^{n-1} a(i)) - n| for n >=
 *            3
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   // 1‑based storage
    static idx_t max_computed = 0;
    static seq_t sum = 0;         // sum of a(1)..a(max_computed)

    if (n == 0) return 0;         /* not part of the sequence, small safety */

    /* Grow cache to accommodate the new index if needed */
    if (cache == NULL || n + 1 > max_computed + 1) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1)
            val = 1;
        else if (k == 2)
            val = 2;
        else {
            seq_t diff = sum > (seq_t)k ? sum - (seq_t)k
                                        : (seq_t)k > sum ? (seq_t)k - sum
                                                        : 0;
            val = diff;
        }
        cache[k] = val;
        sum += val;
    }

    max_computed = n;
    return cache[n];
}
