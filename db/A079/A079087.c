/*
 * A079087  Maximum exponent in prime factorization of
 *          (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is the largest exponent in the prime factorisation of
 *            ((prime(n)+1)*(prime(n+1)+1))/4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses the provided helpers:
    - get_prime(n): returns the n-th prime (1‑based).
    - factor_uint64(x, fac, exp, cnt): factorises x ≤ 2^64-1. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    /*  OEIS uses offset 1, so n ≥ 1. For n == 0 we return 0. */
    if (n == 0) return 0;

    /* Retrieve the n‑th and (n+1)‑th primes. */
    uint64_t p = get_prime(n);
    uint64_t q = get_prime(n + 1);

    /* Compute ((p+1)*(q+1))/4.  Use 128‑bit arithmetic to avoid overflow. */
    __int128 prod128 = (__int128)(p + 1) * (q + 1);
    prod128 >>= 2;                     /* divide by 4 (exact division). */
    uint64_t prod = (uint64_t)prod128; /* fits in 64 bits for all n that fit into seq_t */

    /* Factorise the product. */
    uint64_t fac[20];
    int       exp[20];
    int       cnt = 0;
    factor_uint64(prod, fac, exp, &cnt);

    /* Find the maximum exponent. */
    int max_exp = 0;
    for (int i = 0; i < cnt; ++i) {
        if (exp[i] > max_exp) max_exp = exp[i];
    }

    return (seq_t)max_exp;
}
