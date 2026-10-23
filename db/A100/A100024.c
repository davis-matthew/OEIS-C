/*
 * A100024  Molien series for complete weight enumerators of trace-additive
 *          Hermitian self-dual codes over the Galois ring GR(4,2).
 * https://oeis.org/A100024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies the linear recurrence
 *   a(n) = u1(n) - Σ_{k=1}^{25} d[k]·a(n-k),
 * where u1(t) = 1 + 9 t² + 27 t³ + 27 t⁴ + 27 t⁵ + 27 t⁶ + 9 t⁷ + t⁹,
 * and d(k) are the coefficients of (1−t)⁷(1−t²)⁹
 * (d[0] = 1, d[1] … d[25] as computed below). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* coefficients of the denominator D(t) = (1-t)^7(1-t^2)^9 */
    static int d[26];
    static int d_initialized = 0;

    /* coefficients of the numerator u1(t) */
    static const seq_t u1[10] = {
        1, 0, 9, 27, 27, 27, 27, 9, 0, 1
    };

    /* cache of computed coefficients */
    static size_t cache_size = 0;      /* number of elements cached (max n+1) */
    static seq_t *cache = NULL;        /* array of a(n) values */

    /* ----- Initialize the denominator coefficients once ----- */
    if (!d_initialized) {
        const int C7[8] = {1, 7, 21, 35, 35, 21, 7, 1};
        const int C9[10] = {1, 9, 36, 84, 126, 126, 84, 36, 9, 1};

        for (int k = 0; k < 26; ++k) d[k] = 0;
        for (int i = 0; i <= 7; ++i) {
            for (int j = 0; j <= 9; ++j) {
                int k = i + 2 * j;
                int sign = ((i + j) & 1) ? -1 : 1;
                d[k] += sign * C7[i] * C9[j];
            }
        }
        d_initialized = 1;
    }

    /* ----- Ensure the cache is large enough ----- */
    if (n >= cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* memory exhaustion */
        cache = new_cache;

        /* Compute a(i) for all i from previous cache_size to n */
        for (size_t i = cache_size; i <= n; ++i) {
            seq_t term = 0;
            if (i < 10) term = u1[i];     /* u1(i) for i <= 9, else 0 */

            /* compute sum_{k=1}^{25} d[k] * a(i-k) */
            seq_t sum = 0;
            for (int k = 1; k <= 25 && (size_t)k <= i; ++k) {
                /* d[k] can be negative; multiplication is defined for seq_t */
                sum += cache[i - k] * (seq_t)d[k];
            }

            /* a(i) = u1(i) - sum   (d[0] == 1) */
            cache[i] = term - sum;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
