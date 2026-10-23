/*
 * A155479  Prime(n) without odd prime digits of n.
 * https://oeis.org/A155479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   nth_allowed_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime(i) where i is the n-th positive integer that contains
   no decimal digits 3, 5 or 7.  */

// Standard headers – allowed by the build system
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/nth_allowed_index.h"

// Largest Prime Estimation for the i-th prime (Rosser's bound)
static idx_t prime_upper_bound(idx_t i) {
    if (i < 6) return 15;
    double di = (double)i;
    return (idx_t)(di * (log(di) + log(log(di))) + 3.0);
}

seq_t a(idx_t n) {
    // Static cache of primes
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t sieve_lim = 0;

    if (n < 1) return 0;                // Undefined for n = 0 or negative

    idx_t target_i = nth_allowed_index(n);   // index in the global prime sequence

    // Grow prime cache if necessary
    if (primes_cnt < target_i) {
        idx_t bound = prime_upper_bound(target_i);
        if (bound < sieve_lim) bound = sieve_lim;
        size_t lim = (size_t)(bound + 1);

        bool *is_prime = (bool *)malloc(lim);
        if (!is_prime) return 0;       // Allocation failure

        for (size_t i = 0; i < lim; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (size_t i = 2; i * i < lim; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j < lim; j += i) is_prime[j] = false;
            }
        }

        // Ensure the primes array can hold all needed primes
        size_t needed = (size_t)target_i;
        if (!primes) {
            primes = (seq_t *)malloc(needed * sizeof(seq_t));
            if (!primes) { free(is_prime); return 0; }
        } else if (primes_cnt < needed) {
            seq_t *tmp = (seq_t *)realloc(primes, needed * sizeof(seq_t));
            if (!tmp) { free(is_prime); return 0; }
            primes = tmp;
        }

        // Collect primes up to the required index
        size_t idx = 0;
        for (size_t i = 2; i < lim && idx < needed; ++i) {
            if (is_prime[i]) {
                primes[idx++] = (seq_t)i;
            }
        }
        primes_cnt = idx;
        sieve_lim = bound;

        free(is_prime);
    }

    return primes[(size_t)target_i - 1];
}
