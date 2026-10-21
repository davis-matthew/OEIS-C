/*
 * A217317  Number of primes between n^2 and n^2 + log_2(n)^2 (inclusive).
 * https://oeis.org/A217317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217317
 *
 *  a(n) = number of primes p with n^2 ≤ p ≤ ⌊n^2 + (log₂ n)²⌋ ,
 *  where log₂ denotes the real base‑2 logarithm.
 *
 *  The implementation uses the supplied generic primality test
 *  `bool is_prime(idx_t x)` (include "../utils/is_prime.h") to
 *  test each integer in the short interval of length
 *  ⌊(log₂ n)²⌋ + 1.  Because (log₂ n)² ≤ 200 for the n values
 *  considered here, the loop is tiny and fast even for arbitrary
 *  orders of calls.  No global state is required.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>          /* for log2() */
#include "../utils/is_prime.h"

/* External routine for checking primality of an arbitrary uint64_t
 * (provided by the library that supplies the utilities). */
bool is_prime(idx_t n);

seq_t a(idx_t n)
{
    /* Compute the interval bounds in 64‑bit arithmetic.  The product
     * n*n never overflows for n up to about 4 294 967 295 (well beyond
     * the n used by the test harness). */
    uint64_t nn = (uint64_t)n;
    uint64_t low = nn * nn;

    /* Compute floor((log₂ n)²) using double precision.
     * The added epsilon guarantees that an integer square root
     * boundary does not incorrectly round up. */
    double log2_n = log2((double)nn);
    double sq = log2_n * log2_n;
    uint64_t add = (uint64_t)(sq + 1e-12);

    uint64_t high = low + add;

    /* Count primes in the inclusive interval [low, high]. */
    seq_t count = 0;
    for (uint64_t p = low; p <= high; ++p) {
        if (is_prime((idx_t)p))
            ++count;
    }
    return count;
}
