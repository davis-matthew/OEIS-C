/*
 * A085045  Smallest k such that tau(n + k) = tau(n*k), or 0 if no such number
 *          exists, where tau = A000005.
 * https://oeis.org/A085045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085045: Smallest k such that tau(n + k) = tau(n * k),
   where tau is the divisor function.  Return 0 if no such k exists. */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1, but guard anyway */

    /* Search for the smallest k > 0 satisfying tau(n+k) = tau(n*k).
       If no such k is found up to MAX_K, return 0. */
    const idx_t MAX_K = 10000000;   /* generous upper bound for safety */

    for (idx_t k = 1; k <= MAX_K; ++k) {
        uint64_t n_plus_k  = (uint64_t)n + (uint64_t)k;
        uint64_t n_times_k = (uint64_t)n * (uint64_t)k;

        seq_t tau1 = divisor_count_u64(n_plus_k);
        seq_t tau2 = divisor_count_u64(n_times_k);

        if (tau1 == tau2) return (seq_t)k;
    }

    return 0;  /* no suitable k found within the bound */
}
