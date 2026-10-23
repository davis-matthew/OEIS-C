/*
 * A165282  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=15, a(2)=210, a(3)=2940, a(4)=41160, a(5)=576240,
 *            a(6)=8067360, a(7)=112943040, a(8)=1581202560. For n = 9: a(9) =
 *            1 + 13*(a(1)+...+a(8)) - 91*a(0). For n >= 10: a(n) =
 *            13*(a(n-1)+...+a(n-8)) - 91*a(n-9). This recurrence follows from
 *            the generating function (t^9 + 2*t^8 + ... + 2*t +...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Fixed initial values (indices 0..8). */
    static const seq_t init[9] = {
        (seq_t)1ULL,
        (seq_t)15ULL,
        (seq_t)210ULL,
        (seq_t)2940ULL,
        (seq_t)41160ULL,
        (seq_t)576240ULL,
        (seq_t)8067360ULL,
        (seq_t)112943040ULL,
        (seq_t)1581202560ULL
    };

    /* Fast return for indices <= 8. */
    if (n < 9)
        return init[n];

    /* Allocate or re‑allocate cache to hold values up to n. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        cache = (seq_t *)tmp;
        cache_size = new_size;
    }

    /* Initialize the first 9 entries. */
    for (idx_t i = 0; i < 9; ++i)
        cache[i] = init[i];

    /* Compute values from 9 up to n. */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t sum = 0;
        /* Sum the previous 8 terms a(k-1) .. a(k-8). */
        for (int i = 1; i <= 8; ++i)
            sum += cache[k - i];

        if (k == 9) {
            /* Special numerator term for a(9). */
            cache[k] = sum * (seq_t)13 - cache[k - 9] * (seq_t)91 + (seq_t)1;
        } else {
            /* Recurrence for k >= 10. */
            cache[k] = sum * (seq_t)13 - cache[k - 9] * (seq_t)91;
        }
    }

    return cache[n];
}
