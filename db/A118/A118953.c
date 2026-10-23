/*
 * A118953  Number of ways to write the n-th prime as 2^k + p, where p is
 *          prime and p < 2^k.
 * https://oeis.org/A118953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of the n-th prime as 2^k + p
 *            with p prime and p < 2^k. The count is 0 or 1 for all n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out of OEIS range, safe return */

    uint32_t prime_n  = get_prime(n);      /* the n-th prime, n ≥ 1 */
    seq_t    count    = 0;
    uint64_t pow2     = 1;                 /* 2^k */

    /* iterate while 2^k < prime_n */
    while (pow2 < (uint64_t)prime_n) {
        uint32_t p_sub = prime_n - (uint32_t)pow2;    /* candidate p' = prime_n - 2^k */

        /* p' must be prime and < 2^k */
        if (p_sub < (uint32_t)pow2 && is_prime((idx_t)p_sub))
            ++count;          /* at most one representation, but count generically */

        pow2 <<= 1;          /* next power of two */
    }

    return count;
}
