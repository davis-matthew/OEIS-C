/*
 * A201182  Primes of the form 7*k^3+1.
 * https://oeis.org/A201182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 7*k^3 + 1 (OEIS A201182)
 * Range:     u8 none, u16 n <= 3, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime of the form 7*k^3+1, with a(1) being the first */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    seq_t value;

    while (count < n) {
        /* Compute 7*k^3 + 1 using 128‑bit intermediate to avoid overflow */
        seq_t kk = (seq_t)k * k;           // k^2
        seq_t kkk = kk * k;               // k^3
        value = 7 * kkk + 1;

        /* Prime test on the 64‑bit representation (value fits in uint64_t) */
        if (is_prime((uint64_t)value)) {
            ++count;
            if (count == n)
                return value;
        }
        ++k;
    }
    /* Should never reach here */
    return 0;
}
