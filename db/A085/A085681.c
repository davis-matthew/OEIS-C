/*
 * A085681  Integers of the form 2^k*p where p is a prime > 2^k.
 * https://oeis.org/A085681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer of the form 2^k * p where p is a prime
 *            greater than 2^k. The function scans even integers in increasing
 *            order and counts those that satisfy the condition
 * Range:     u8 n <= 53, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t hit = 0;
    uint64_t val = 6;          /* first term in the sequence */
    while (1) {
        unsigned k = __builtin_ctzll(val);   /* largest power of two dividing val (k >= 1) */
        uint64_t p = val >> k;               /* candidate prime factor */

        if (p > ((uint64_t)1 << k) && is_prime_small((seq_t)p)) {
            ++hit;
            if (hit == n)
                return (seq_t)val;
        }
        val += 2;  /* next even integer */
    }
}
