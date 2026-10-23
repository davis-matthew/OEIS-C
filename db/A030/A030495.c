/*
 * A030495  a(n) = (n+1)! + n.
 * https://oeis.org/A030495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)! + n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute factorial (n+1)! with a lightweight cache. */
    static idx_t max_fact_n = 0;          /* index of the largest cached factorial */
    static seq_t max_fact_val = (seq_t)1; /* value of max_fact_n! (always !0 = 1) */

    idx_t k = n + 1;                      /* we need (n+1)! */

    if (k > max_fact_n) {
        /* Extend the cache up to k */
        for (idx_t i = max_fact_n + 1; i <= k; ++i) {
            max_fact_val = max_fact_val * (seq_t)i;
        }
        max_fact_n = k;
    }

    seq_t fact_k;
    if (k == max_fact_n) {
        fact_k = max_fact_val;
    } else {
        /* Compute factorial for a smaller k without using cache */
        fact_k = (seq_t)1;
        for (idx_t i = 2; i <= k; ++i) {
            fact_k = fact_k * (seq_t)i;
        }
    }

    return fact_k + (seq_t)n;
}
