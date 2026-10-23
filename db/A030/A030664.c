/*
 * A030664  Product of largest prime <= n and smallest prime >= n.
 * https://oeis.org/A030664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (largest prime ≤ n) * (smallest prime ≥ n) with a(0) = 1,
 *            a(1) = 1 (by definition)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper functions from the utilities are available:
   - bool is_prime(idx_t x)          (in "../utils/is_prime.h")
   - uint64_t next_prime(uint64_t start)  (in "../utils/next_prime.h")
   - uint64_t prev_prime(idx_t n)   (in "../utils/prev_prime.h")
   Since these routines may not be present in all builds, the implementation
   below uses only is_prime() and performs a simple search; it works for all
   supported build types. */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute product of largest prime ≤ n and smallest prime ≥ n.
   For n = 0 or 1 the sequence is defined to be 1. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t lower = 0;
    seq_t upper = 0;

    /* Find largest prime <= n. */
    for (idx_t x = n; x >= 2; --x) {
        if (is_prime(x)) {
            lower = (seq_t)x;
            break;
        }
        if (x == 2) break;   /* prevent underflow on unsigned type */
    }

    /* Find smallest prime >= n. */
    for (idx_t x = n; ; ++x) {
        if (is_prime(x)) {
            upper = (seq_t)x;
            break;
        }
    }

    /* n >= 2 guarantees both primes found. */
    return lower * upper;
}
