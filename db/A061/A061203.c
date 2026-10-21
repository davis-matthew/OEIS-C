/*
 * A061203  (tau<=)_5(n).
 * https://oeis.org/A061203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} tau_4(k) * floor(n/k), where tau_4(k) is the
 *            number of ordered quadruples of positive integers whose product
 *            equals k
 * Range:     u8 n <= 14, u16 n <= 407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A061203.
   It maintains internal caches for divisor‑count functions
   up to the largest n requested so far. */

#include <stdlib.h>   /* for malloc, free, size_t */

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* static state */
    static idx_t max_n = 0;
    static seq_t *tau2 = NULL;   /* tau_2: number of divisors */
    static seq_t *tau3 = NULL;   /* tau_3: ordered triples product = k */
    static seq_t *tau4 = NULL;   /* tau_4: ordered quadruples product = k */

    /* If requested n exceeds current cache, recompute up to n */
    if (n > max_n) {
        idx_t new_n = n;

        /* Allocate new arrays (size new_n + 1 to use 1‑based indexing) */
        seq_t *t2 = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
        seq_t *t3 = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
        seq_t *t4 = (seq_t*)calloc(new_n + 1, sizeof(seq_t));

        if (!t2 || !t3 || !t4) {
            /* Allocation failure: return 0 (unlikely in contest environment) */
            if (t2) free(t2);
            if (t3) free(t3);
            if (t4) free(t4);
            return (seq_t)0;
        }

        /* Compute tau_2 (divisor count) */
        for (idx_t i = 1; i <= new_n; ++i) {
            for (idx_t j = i; j <= new_n; j += i) {
                ++t2[j];
            }
        }

        /* Compute tau_3 from tau_2 */
        for (idx_t i = 1; i <= new_n; ++i) {
            for (idx_t j = i; j <= new_n; j += i) {
                t3[j] += t2[i];
            }
        }

        /* Compute tau_4 from tau_3 */
        for (idx_t i = 1; i <= new_n; ++i) {
            for (idx_t j = i; j <= new_n; j += i) {
                t4[j] += t3[i];
            }
        }

        /* Free old arrays */
        if (tau2) free(tau2);
        if (tau3) free(tau3);
        if (tau4) free(tau4);

        /* Store new arrays and update cache size */
        tau2 = t2; tau3 = t3; tau4 = t4;
        max_n = new_n;
    }

    /* Compute a(n) = sum_{k=1..n} tau_4(k) * floor(n/k) */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        result += tau4[k] * (n / k);
    }

    return result;
}
