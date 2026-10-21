/*
 * A068550  a(n) = lcm{1, ..., 2n} / binomial(2n, n).
 * https://oeis.org/A068550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm{1,…,2n} / binomial(2n,n)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

/* integer square root for small n */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* OEIS A068550 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t limit = 2 * n;          /* 2n */
    size_t sz = (size_t)(limit + 1);

    /* simple sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return (seq_t)0;          /* memory error – return 0 */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    idx_t r = isqrt_u64(limit);
    for (idx_t p = 2; p <= r; ++p) {
        if (!is_prime[p])
            continue;
        for (idx_t m = p * p; m <= limit; m += p)
            is_prime[m] = false;
    }

    seq_t result = (seq_t)1;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;

        /* exponent of p in lcm(1..2n) */
        unsigned max_exp = 0;
        idx_t pp = p;
        while (pp <= limit) {
            ++max_exp;
            if (pp > limit / p)
                break;
            pp *= p;
        }

        /* exponent of p in binomial(2n,n) */
        unsigned bin_exp = 0;
        idx_t pk = p;
        while (pk <= limit) {
            bin_exp += (limit / pk) - 2 * (n / pk);
            if (pk > limit / p)
                break;
            pk *= p;
        }

        int diff = (int)max_exp - (int)bin_exp;
        if (diff <= 0)
            continue;

        seq_t mult = pow_uint((uint64_t)p, (idx_t)diff);
        result = seq_mul(result, mult);
    }

    free(is_prime);
    return result;
}
