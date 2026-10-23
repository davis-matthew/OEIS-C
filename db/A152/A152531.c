/*
 * A152531  a(n) = (p(n)*p(n+2) - p(n+1))/2, where p(n) is the n-th odd prime.
 * https://oeis.org/A152531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p(n)*p(n+2) - p(n+1)) / 2 * where p(k) denotes the k-th
 *            odd prime, i.e. p(k) = get_prime(k+1) (get_prime counts all
 *            primes starting at 2)
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* offset for odd primes: p(1) is the 2nd prime (=3) */
    idx_t offset = n + 1;          /* get_prime(index) uses 1-based indexing */
    uint32_t p1 = get_prime(offset);    /* p(n)   */
    uint32_t p2 = get_prime(offset + 1);/* p(n+1) */
    uint32_t p3 = get_prime(offset + 2);/* p(n+2) */

    seq_t prod = (seq_t)p1 * (seq_t)p3; /* use seq_t for intermediate */
    seq_t res  = (prod - (seq_t)p2) / 2;
    return res;
}
