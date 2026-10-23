/*
 * A133992  a(n) = tau( prime(n)*prime(prime(n)) - 1 ).
 * https://oeis.org/A133992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = τ(prime(n)*prime(prime(n)) - 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    uint64_t p_n  = (uint64_t)get_prime(n);
    uint64_t p_pk = (uint64_t)get_prime(p_n);
    __uint128_t prod = (__uint128_t)p_n * p_pk;
    uint64_t val = (uint64_t)(prod - 1);
    return divisor_count_u64(val);
}
