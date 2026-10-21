/*
 * A093008  Smallest number of 1's to append to the binary representation of n
 *          such that primes become nonprimes and nonprimes become primes.
 * https://oeis.org/A093008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 1 such that if n is prime, ((n << k)+(2^k-1))
 *            is composite; * otherwise ((n << k)+(2^k-1)) is prime
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Computes a(n) for the OEIS sequence A093008. */
seq_t a(idx_t n)
{
    /* Treat n as a 64‑bit unsigned integer. */
    uint64_t n_val = (uint64_t)n;
    bool n_prime   = is_prime(n_val);

    /* Use 128‑bit arithmetic to avoid overflow while appending 1's. */
    unsigned __int128 val = (unsigned __int128)n_val;

    for (idx_t k = 1; ; ++k) {
        /* Append a single binary '1': val = val * 2 + 1. */
        val = val * 2 + 1;

        /* For safety, only test primality if the value fits in 64 bits. */
        bool val_prime = false;
        if (val <= UINT64_MAX) {
            val_prime = is_prime((idx_t)val);
        } else {
            /* The required k for n ≤ 10000 will never drive us here. */
            /* If we do, assume val is composite to avoid infinite loop. */
            val_prime = false;
        }

        /* Check the switching condition. */
        if ((n_prime && !val_prime) || (!n_prime && val_prime))
        {
            return (seq_t)k;
        }
    }
}
