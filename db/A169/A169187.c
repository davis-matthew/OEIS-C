/*
 * A169187  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 * a(0)=1,
 * for 1 ≤ n ≤ 26:   a(n)=28·∑_{k=1}^{n}a(n‑k)+2
 * a(27) = 28·∑_{k=1}^{26}a(27‑k) – 406·a(0) + 1
 * for n ≥ 28: a(n)=28·∑_{k=1}^{26}a(n‑k) – 406·a(n‑27)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* caching of already computed terms */
    static seq_t *cache = NULL;          /* array of values a[0..] */
    static size_t capacity = 0;          /* allocated capacity (elements) */
    static idx_t  next = 0;              /* index of first element not yet computed */

    /* enlarge storage if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = ((size_t)n + 1 > 2 * capacity) ? (size_t)n + 1 : 2 * capacity + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t value;

        if (i == 0) {
            value = (seq_t)1;
        } else {
            seq_t sum = 0;
            /* add the previous at most 26 terms */
            idx_t m = (i >= 26) ? 26 : i;
            for (idx_t k = 1; k <= m; ++k) {
                sum += cache[(size_t)(i - k)];
            }
            value = 28 * sum;           /* 28 * sum_{k=1}^{min(26,i)} a[i-k] */

            if (i >= 27) {              /* subtract 406 * a[i-27] if defined */
                value -= (seq_t)406 * cache[(size_t)(i - 27)];
            }

            if (i <= 26) {              /* coefficient from numerator */
                value += (seq_t)2;       /* for i=1..26: coefficient 2 */
            } else if (i == 27) {
                value += (seq_t)1;       /* for i=27: coefficient 1 */
            }
        }

        cache[(size_t)i] = value;
    }

    next = n + 1;
    return cache[(size_t)n];
}
