/*
 * A170243  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence for A170243 (Number of reduced words of length n in the
 * Coxeter group with 42 generators S_i, relations (S_i)^2 = (S_i S_j)^40 = I).
 *
 * Let a(0) = 1.
 * For 1 ≤ n ≤ 39:      a(n) = 2 + 40 * Σ_{k=0}^{n-1} a(k).
 * For n = 40:          a(40) = 40 * Σ_{k=0}^{39} a(k) – 859.
 * For n ≥ 41:          a(n) = 40 * Σ_{k=n-39}^{n-1} a(k) – 820 * a(n-40).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t computed = 0;   /* number of elements presently stored (0..computed-1) */

    if (n >= capacity) {             /* enlarge the cache if required */
        size_t newcap = capacity ? capacity * 2 : 1;
        while (newcap <= n) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return zero as a safe fallback */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute missing terms up to n */
    for (size_t k = computed; k <= (size_t)n; ++k) {
        seq_t value;
        if (k == 0) {
            value = (seq_t)1;
        } else if (k <= 39) { /* 1 ≤ k ≤ 39 */
            seq_t sum = 0;
            for (size_t i = 0; i < k; ++i) sum += cache[i];
            value = (seq_t)2 + ((seq_t)40 * sum);
        } else if (k == 40) { /* special formula for a(40) */
            seq_t sum = 0;
            for (size_t i = 0; i <= 39; ++i) sum += cache[i];
            value = (seq_t)40 * sum - (seq_t)859;
        } else { /* k ≥ 41 */
            seq_t sum = 0;
            for (idx_t i = k - 39; i < k; ++i) sum += cache[i];
            value = (seq_t)40 * sum - (seq_t)820 * cache[k - 40];
        }
        cache[k] = value;
    }
    computed = (size_t)n + 1;
    return cache[n];
}
