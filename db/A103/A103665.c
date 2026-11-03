/*
 * A103665  Primes p such that the number of divisors of p-1 is greater than
 *          the number of divisors of p+1.
 * https://oeis.org/A103665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that τ(p‑1) > τ(p+1)
 * Range:     u8 n <= 20, u16 n <= 3058, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9887 (2026-11-03)
 * Helpers:   is_prime_u64, num_divisors_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (τ is the divisor–count function) */

#include <stdint.h>
#include <stdbool.h>
/* Utility functions that are available in the build environment */
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/num_divisors_idx.h"

seq_t a(idx_t n) {
    if (n == 0) {
        /* undefined for n < 1, but return 0 to avoid UB */
        return (seq_t)0;
    }

    idx_t found = 0;
    uint64_t p = 2;          /* start from the first prime */

    while (true) {
        if (is_prime_u64(p)) {
            seq_t d1 = num_divisors_idx((idx_t)(p - 1));
            seq_t d2 = num_divisors_idx((idx_t)(p + 1));
            if (d1 > d2) {
                found++;
                if (found == n) {
                    return (seq_t)p;
                }
            }
        }

        /* Increment to next candidate: skip even numbers after 2 */
        if (p == 2) {
            p = 3;
        } else {
            p += 2;
        }
    }
}
