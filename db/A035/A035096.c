/*
 * A035096  a(n) is the smallest k such that prime(n)*k+1 is prime.
 * https://oeis.org/A035096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer k such that prime(n)*k+1 is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    // n is 1‑based: get the n‑th prime
    idx_t p = (idx_t)get_prime(n);

    // search for the smallest k with p*k+1 prime
    for (idx_t k = 1; ; ++k) {
        idx_t cand = p * k + 1;
        if (is_prime(cand))
            return (seq_t)k;
    }
}
