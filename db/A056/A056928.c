/*
 * A056928  Average of the smallest prime greater than n^2 and the largest
 *          prime less than n^2.
 * https://oeis.org/A056928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (next_prime(n^2) + prev_prime(n^2)) / 2 * where
 *            prev_prime(n^2) is the largest prime < n^2 * and next_prime(n^2)
 *            is the smallest prime > n^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Undefined behavior for n < 2 is not relevant; return 0 */
    if (n < 2)
        return (seq_t)0;

    /* Compute n^2 in a 64‑bit unsigned integer */
    uint64_t sq = (uint64_t)n * (uint64_t)n;

    /* Find the largest prime strictly less than n^2 */
    uint64_t prev = 0;
    uint64_t x = sq - 1;
    while (true) {
        if (is_prime(x)) {
            prev = x;
            break;
        }
        if (x <= 2) { /* reached the lowest candidate */
            /* No prime found below 2; this should never happen for n ≥ 2 */
            break;
        }
        --x;
    }

    /* Find the smallest prime strictly greater than n^2 */
    uint64_t next = 0;
    x = sq + 1;
    while (true) {
        if (is_prime(x)) {
            next = x;
            break;
        }
        ++x;
    }

    /* Compute the average; the sum of two odd primes is even,
       so the division is exact in integer arithmetic. */
    seq_t avg = ((seq_t)prev + (seq_t)next) / 2;
    return avg;
}
