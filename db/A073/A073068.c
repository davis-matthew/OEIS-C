/*
 * A073068  Minimum value of abs(n-x*tau(x)) x>0.
 * https://oeis.org/A073068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A073068.
   a(n) = min_{x>=1} | n - x * tau(x) |,  tau(x) = number of divisors of x.
   The function computes tau(x) up to the maximum n seen and then
   evaluates the minimum difference by brute force (x ranges from 1 to n),
   which is sufficient because for x > n, x*tau(x) > n. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for divisor counts up to the biggest n requested so far */
    static idx_t *tau = NULL;   /* tau[0] unused, tau[i] = d(i) */
    static idx_t tau_len = 0;   /* current size of the cache (max i with tau[i] defined) */

    /* recompute tau array if necessary */
    if (n > tau_len) {
        idx_t new_len = n;
        /* free previous cache if any */
        if (tau != NULL) {
            free(tau);
        }
        /* allocate inclusive of index 1..new_len */
        tau = (idx_t*)calloc((size_t)new_len + 1, sizeof(idx_t));
        if (tau == NULL) {        /* allocation failed – return zero as a safe value */
            return (seq_t)0;
        }

        /* compute tau using the classic O(N log N) sieve */
        for (idx_t i = 1; i <= new_len; ++i) {
            for (idx_t j = i; j <= new_len; j += i) {
                tau[j] += 1;
            }
        }
        tau_len = new_len;
    }

    /* compute the minimal absolute difference */
    seq_t min_diff = (seq_t)n;   /* initial upper bound: |n-1| at least n-1, but n works as a safe overestimate */
    for (idx_t x = 1; x <= n; ++x) {
        seq_t product = (seq_t)x * (seq_t)tau[x];
        seq_t diff = product > n ? product - n : n - product;
        if (diff < min_diff) {
            min_diff = diff;
            if (min_diff == 0) break;   /* cannot beat zero */
        }
    }
    return min_diff;
}
