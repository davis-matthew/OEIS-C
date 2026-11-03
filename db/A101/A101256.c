/*
 * A101256  Sum of composites <= n.
 * https://oeis.org/A101256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - sum_{p ≤ n} p - 1 (composite sums)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t sum_primes_upto(idx_t limit) {
    if (limit < 2) return (seq_t)0;
    /* Allocate and build a simple sieve */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime) return (seq_t)0;           /* fallback – memory failure */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }
    seq_t sum = (seq_t)0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) sum += (seq_t)i;
    free(is_prime);
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* safety for out‑of‑range calls */
    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;
    seq_t prime_sum = sum_primes_upto(n);
    return tri - prime_sum - (seq_t)1;
}
