/*
 * A020482  Greatest p with p, q both prime, p+q = 2n.
 * https://oeis.org/A020482
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that p and (2n - p) are primes
 *            (Goldbach partition). The function performs a manual search from
 *            the largest possible candidate downwards
 * Range:     u8 n <= 129, u16 n <= 8566, u32 n <= 8566, u64 n <= 8566,
 *            u128 n <= 8566, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;            /* undefined for n < 2, safety fallback */
    idx_t target = n << 1;                 /* 2n */
    for (idx_t p = target - 2; p >= 2; --p) {
        if (is_prime(p) && is_prime(target - p)) {
            return (seq_t)p;
        }
    }
    return (seq_t)0;                       /* should never be reached */
}
