/*
 * A023510  Greatest exponent in prime-power factorization of prime(n) + 1.
 * https://oeis.org/A023510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest exponent in prime‑power factorization of
 *            prime(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* in case of a call with n==0 */

    /* Get the n‑th prime (1–based) and add 1. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t x = p + 1ULL;                 /* value to factor */

    seq_t max_exp = 1;                     /* at least one */

    /* Small statically sized buffers suffice for the factorisation of
     * values up to the 10 000th prime (~10⁵). */
    uint64_t pr[32];
    uint64_t ex[32];
    size_t cnt = 0;

    factor_u64(x, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        if (ex[i] > max_exp) max_exp = (seq_t)ex[i];
    }

    return max_exp;
}
