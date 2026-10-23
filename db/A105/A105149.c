/*
 * A105149  Number of even semiprimes k such that n^2 < k <= (n+1)^2.
 * https://oeis.org/A105149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3764, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the number of even semiprimes k=2p with p prime such that
 * n^2 < k <= (n+1)^2.
 * This is equivalent to counting primes p in the interval
 * (n^2/2, (n+1)^2/2].
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"

seq_t a(idx_t n) {
    /* For n == 0 the interval is (0,1], no even semiprime */
    if (n == 0) return 0;

    /* Static state to keep the prime list up to the largest required limit */
    static uint32_t     current_limit = 0;   /* maximum value we have sieved up to */
    static uint32_t    *prime_list = NULL;   /* array of primes */
    static size_t       prime_count = 0;     /* number of primes in prime_list */

    /* Compute the required high bound for primes: (n+1)^2 / 2 */
    uint64_t high = ((uint64_t)(n + 1) * (n + 1)) / 2;

    /* Re‑sieve if we need a larger limit */
    if (high > current_limit) {
        free(prime_list);
        simple_sieve((uint32_t)high, &prime_list, &prime_count);
        current_limit = (uint32_t)high;
    }

    /* Lower bound for p: strictly greater than n^2/2 */
    uint64_t low = ((uint64_t)n * n) / 2 + 1;
    if (low > high) return 0;   /* empty interval */

    /* Binary search for first prime >= low */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint32_t)prime_list[mid] < low)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t first = lo;

    /* Binary search for first prime > high */
    lo = 0; hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint32_t)prime_list[mid] <= high)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t last = lo;

    /* Number of primes in [low, high] */
    return (seq_t)(last - first);
}
