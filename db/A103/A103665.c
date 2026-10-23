/*
 * A103665  Primes p such that the number of divisors of p-1 is greater than
 *          the number of divisors of p+1.
 * https://oeis.org/A103665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that τ(p‑1) > τ(p+1)
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   count_divisors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (τ is the divisor–count function) */

#include <stdint.h>
#include <stdbool.h>
/* Utility functions that are available in the build environment */
#include "../utils/is_prime.h"          /* bool is_prime(uint64_t n); */
#include "../utils/count_divisors.h"    /* seq_t count_divisors(idx_t n); */

seq_t a(idx_t n) {
    if (n == 0) {
        /* undefined for n < 1, but return 0 to avoid UB */
        return (seq_t)0;
    }

    idx_t found = 0;
    uint64_t p = 2;          /* start from the first prime */

    while (true) {
        if (is_prime(p)) {
            seq_t d1 = count_divisors((idx_t)(p - 1));
            seq_t d2 = count_divisors((idx_t)(p + 1));
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
