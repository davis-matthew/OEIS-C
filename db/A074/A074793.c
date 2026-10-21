/*
 * A074793  Sum of prime powers less than or equal to n.
 * https://oeis.org/A074793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 586, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{p prime, k≥1} p^k  (over all prime powers p^k ≤ n)
 * The function computes the sum of all prime powers not exceeding n.
 * It uses a simple sieve to generate the primes up to n (cached across calls)
 * and then adds each prime power p^k until it would exceed n.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Cached prime list up to the largest n seen. */
static idx_t cached_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* a(1) = 0 */

    /* Rebuild sieve if needed. */
    if (n > cached_limit) {
        /* Free previous storage. */
        free(prime_list);
        prime_list = NULL;

        /* Build up to n. */
        build_primes_upto(n, &prime_list, &prime_count);
        cached_limit = n;
    }

    /* Sum all prime powers <= n. */
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (size_t idx = 0; idx < prime_count; ++idx) {
        seq_t power = prime_list[idx];
        while (power <= n_seq) {
            sum += power;
            power *= prime_list[idx];
        }
    }
    return sum;
}
