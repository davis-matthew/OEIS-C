/*
 * A170118  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence deriving from its rational
 *            generating function. a(n) = 11*a(n-1) + 11*a(n-2) + … +
 *            11*a(n-37) - 66*a(n-38) for n >= 38 initial values a(0)=1,
 *            a(n)=2 + 11*(a(n-1)+…+a(0)) for 1 <= n <= 38
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc */

static seq_t *cache = NULL;   /* memoisation table */
static size_t capacity = 0;   /* allocated size of cache */
static idx_t max_computed = 0;

/* Compute all terms up to n incrementally, keeping them in cache. */
static void compute_up_to(idx_t n) {
    if (n <= max_computed) return;

    /* ensure sufficient storage */
    if (n + 1 > capacity) {
        size_t new_cap = capacity > 0 ? capacity : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = seq_realloc(cache, capacity, new_cap);
        capacity = new_cap;
    }

    /* initialise the first value if this is the first call */
    if (max_computed == 0 && capacity > 0) {
        cache[0] = (seq_t)1;
    }

    /* compute values from max_computed + 1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i <= 38) {
            /* recurrence with non‑zero RHS: a(i) = 2 + 11*(a(i-1)+…+a(0)) */
            for (idx_t k = 1; k <= i; ++k) {
                sum += (seq_t)11 * cache[i - k];
            }
            sum += (seq_t)2;
        } else {
            /* homogeneous recurrence of order 38 */
            for (idx_t k = 1; k <= 37; ++k) {
                sum += (seq_t)11 * cache[i - k];
            }
            sum -= (seq_t)66 * cache[i - 38];
        }
        cache[i] = sum;
    }
    max_computed = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* base case */
    compute_up_to(n);
    return cache[n];
}
