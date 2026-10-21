/*
 * A229657  7-section a(7n+k) gives k-th differences of a for k=0..6 with
 *          a(n)=0 for n<6 and a(6)=1.
 * https://oeis.org/A229657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen, look
 * Formula:   a(7*n+k) = Σ_{j=0..k} (-1)^j * C(k,j) * a(n + k - j) with a(n) =
 *            0 for n < 6 and a(6) = 1
 * Range:     i8 n <= 75, i16 n <= 2910, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t next_compute = 0;  /* next index that needs to be computed */

    /* Ensure the cache can hold a[n] */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    if (next_compute <= (size_t)n) {
        for (idx_t i = next_compute; i <= n; ++i) {
            if (i < 6) {
                cache[(size_t)i] = 0;
            } else if (i == 6) {
                cache[(size_t)i] = 1;
            } else {
                idx_t block = i / 7;
                idx_t k     = i % 7;
                seq_t sum   = 0;
                for (idx_t j = 0; j <= k; ++j) {
                    /* multiplicative binomial C(k,j) */
                    seq_t binom = 1;
                    for (idx_t t = 1; t <= j; ++t) {
                        binom = binom * (seq_t)(k - t + 1) / (seq_t)t;
                    }
                    idx_t idx = block + k - j;
                    seq_t term = cache[(size_t)idx] * binom;
                    if (j & 1) sum -= term;
                    else       sum += term;
                }
                cache[(size_t)i] = sum;
            }
        }
        next_compute = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
