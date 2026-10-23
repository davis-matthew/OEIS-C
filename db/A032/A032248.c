/*
 * A032248  "DHK[ 7 ]" (bracelet, identity, unlabeled, 7 parts) transform of
 *          1,1,1,1,...
 * https://oeis.org/A032248
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑1) – 8*a(n‑3) + 6*a(n‑4) + 6*a(n‑5) – 8*a(n‑6) * +
 *            a(n‑7) – a(n‑9) + 8*a(n‑10) – 6*a(n‑11) – 6*a(n‑12) * +
 *            8*a(n‑13) – 3*a(n‑15) + a(n‑16) for n > 25 * Initial values (for
 *            n = 10 … 25): * 4, 10, 28, 56, 113, 197, 340, 544, 856, 1284,
 *            1896, 2709, 3816, 5247, * 7128, 9504
 * Range:     u8 n <= 15, u16 n <= 33, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* maximum n that is needed for the tests (≈10000) */
    #define MAX_N 10000

    /* static cache to store already computed values */
    static seq_t cache[MAX_N + 1];
    static bool initialized = false;
    static idx_t max_computed = 0;

    if (!initialized) {
        /* initialise cache: all zeros for indices < 10 are irrelevant */
        for (idx_t i = 0; i <= MAX_N; ++i)
            cache[i] = 0;

        /* initial values for n = 10 … 25 */
        static const seq_t init[16] = {
            4,   10,  28,  56,  113,  197,  340,  544,
            856, 1284,1896,2709,3816,5247,7128,9504
        };
        for (idx_t i = 0; i < 16; ++i)
            cache[10 + i] = init[i];

        max_computed = 25;
        initialized = true;
    }

    if (n <= max_computed)
        return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* compute the linear combination in an order that avoids
           unsigned underflow (the final result is always non‑negative) */
        seq_t val =
            cache[i-1] * (seq_t)3
          + cache[i-4] * (seq_t)6
          + cache[i-5] * (seq_t)6
          + cache[i-7]
          + cache[i-10] * (seq_t)8
          + cache[i-13] * (seq_t)8
          + cache[i-16];
        val -= cache[i-3] * (seq_t)8
             + cache[i-6] * (seq_t)8
             + cache[i-9]
             + cache[i-11] * (seq_t)6
             + cache[i-12] * (seq_t)6
             + cache[i-15] * (seq_t)3;
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
