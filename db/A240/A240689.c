/*
 * A240689  The number of values of the digit k for which prime(n)*10+k is
 *          prime.
 * https://oeis.org/A240689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k in {1,…,9} such that prime(n)*10+k is prime
 * Range:     u8 n <= 9, u16 n <= 1026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Offset is 1. Return 0 for n==0 (undefined by OEIS). */
    if (n == 0)
        return 0;

    /* The n-th prime, indexed from 1. */
    uint32_t p = get_prime(n);

    seq_t cnt = 0;
    /* Test each possible appended digit k. */
    for (uint8_t k = 1; k <= 9; ++k) {
        uint32_t cand = p * 10u + (uint32_t)k;
        if (is_prime_small((seq_t)cand))
            ++cnt;
    }
    return cnt;
}
