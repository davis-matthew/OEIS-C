/*
 * A079715  a(n) = pi(n) - pi(sqrt(n)) + 1.
 * https://oeis.org/A079715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) - pi(⌊√n⌋) + 1 (pi(x) = number of primes ≤ x)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_pi.h"

/* integer floor square root for unsigned integers */
static idx_t floor_sqrt(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;                     /* floor(sqrt(n)) */
}

/* OEIS sequence A079715 */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 */
    idx_t sq = floor_sqrt(n);
    seq_t pi_n = prime_pi(n);
    seq_t pi_s = prime_pi(sq);
    return pi_n - pi_s + 1;
}
