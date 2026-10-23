/*
 * A163400  Number of bits in binary expansion of n-th nonprime.
 * https://oeis.org/A163400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 9624, u16 n <= 9624, u32 n <= 9624, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   floor_log2_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163400: Number of bits in binary expansion of the n‑th nonprime.
   Computation: enumerate nonprimes (including 0), select the n‑th one,
   and return the size of its binary representation.
   For 0 the length is defined as 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    idx_t count = 0;
    idx_t i = 0;
    idx_t nonprime = 0;

    while (1) {
        if (!is_prime(i)) {       /* nonprime found */
            ++count;
            if (count == n) {
                nonprime = i;
                break;
            }
        }
        ++i;
    }

    if (nonprime == 0)
        return (seq_t)1;          /* 0 in binary is "0" -> 1 bit */

    return (seq_t)(floor_log2_uint64(nonprime) + 1);
}
