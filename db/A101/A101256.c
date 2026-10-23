/*
 * A101256  Sum of composites <= n.
 * https://oeis.org/A101256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - sum_{p ≤ n} p - 1 (composite sums)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   sum_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sum_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* safety for out‑of‑range calls */
    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;
    seq_t prime_sum = sum_primes_upto(n);
    return tri - prime_sum - (seq_t)1;
}
