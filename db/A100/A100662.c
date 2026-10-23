/*
 * A100662  Primes of the form k^3 + (k+1)^2.
 * https://oeis.org/A100662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form k^3 + (k+1)^2, k ≥ 1
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans k in increasing order and counts the primes
 * of the required form until the desired index n is reached.
 * It uses 128‑bit arithmetic to compute k^3 + (k+1)^2 without overflow,
 * then tests primality with the helper is_prime from utils/is_prime.h.
 * All arithmetic stays within the bounds of seq_t (or uint64_t) for the
 * values needed up to n ≈ 10 000, which is well within 64‑bit. */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;   /* how many suitable primes found */
    idx_t k = 1;       /* current k to test */

    while (1) {
        /* Compute k^3 + (k+1)^2 with 128‑bit arithmetic to avoid overflow */
        unsigned __int128 cube  = (unsigned __int128)k * k * k;
        unsigned __int128 square = (unsigned __int128)(k + 1) * (k + 1);
        unsigned __int128 val128 = cube + square;

        /* The result fits in 64‑bit for all k produced when n ≤ 10 000 */
        uint64_t val = (uint64_t)val128;

        /* Test primality */
        if (is_prime(val)) {
            ++count;
            if (count == n) {
                return (seq_t)val;   /* cast to the required return type */
            }
        }
        ++k;
    }
}
