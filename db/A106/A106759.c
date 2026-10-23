/*
 * A106759  Primes with digit sum = 19.
 * https://oeis.org/A106759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 605, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_uint64, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function finds the n-th prime whose digit sum is 19 by brute‑force search. */
#include <stdint.h>
#include "../utils/is_prime_uint64.h"   /* is_prime_uint64(uint64_t) */
#include "../utils/digit_sum_uint64.h" /* digit_sum_uint64(uint64_t) */

seq_t a(idx_t n)
{
    uint64_t p   = 199;   /* first prime with digit sum 19 */
    idx_t    cnt = 0;     /* how many such primes found so far */

    for (;; p += 2)       /* iterate over odd numbers only */
    {
        if (is_prime_uint64(p) && digit_sum_uint64(p) == 19U)
        {
            ++cnt;
            if (cnt == n)
                return (seq_t)p;
        }
    }
}
