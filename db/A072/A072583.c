/*
 * A072583  Numbers k with the property that there is no match when comparing
 *          the numbers of 0's and 1's in the binary representations of k and
 *          the k-th prime.
 * https://oeis.org/A072583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = n-th integer k for which
 *   popcount(k) != popcount(pk)  &&
 *   popcount(k) != zeros(pk)     &&
 *   zeros(k) != popcount(pk)    &&
 *   zeros(k) != zeros(pk),
 *   where pk is the k-th prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – not used, but safe */
    idx_t found = 0;
    idx_t k = 0;
    while (found < n) {
        ++k;
        uint64_t kv  = (uint64_t)k;
        int ones_k   = __builtin_popcountll(kv);
        int len_k    = 64 - __builtin_clzll(kv);
        int zeros_k  = len_k - ones_k;

        uint32_t pk32 = get_prime(k);      /* k‑th prime */
        uint64_t pkv  = (uint64_t)pk32;
        int ones_pk   = __builtin_popcountll(pkv);
        int len_pk    = 64 - __builtin_clzll(pkv);
        int zeros_pk  = len_pk - ones_pk;

        if (ones_k   != ones_pk &&
            ones_k   != zeros_pk &&
            zeros_k  != ones_pk &&
            zeros_k  != zeros_pk) {
            ++found;
        }
    }
    return (seq_t)k;
}
