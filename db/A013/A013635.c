/*
 * A013635  a(n) = prevprime(n) + n.
 * https://oeis.org/A013635
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prevprime(n) + n Implementation: find the largest prime <
 *            n by checking each candidate downward using is_prime, then add n
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;               /* undefined for n < 3, return 0 */
    idx_t p = n - 1;                   /* start one less than n */
    while (!is_prime(p)) {              /* search downward for a prime */
        p--;
    }
    return (seq_t)p + (seq_t)n;        /* combine in seq_t */
}
