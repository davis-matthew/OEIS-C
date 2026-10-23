/*
 * A023577  Greatest exponent in prime-power factorization of prime(n)+3.
 * https://oeis.org/A023577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = greatest exponent in prime-power factorization of
 *            prime(n)+3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) : returns the k‑th prime */
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime (1‑indexed) */
    uint64_t p = (uint64_t)get_prime(n);

    /* Compute m = prime(n) + 3 */
    uint64_t m = p + 3;

    /* Factor m and keep the maximum exponent */
    size_t cnt = 0;
    /* The number of distinct prime factors of an integer <= 104732
       is less than 16, so 64 slots are more than enough. */
    uint64_t pr[64], ex[64];

    factor_u64(m, pr, ex, &cnt);

    seq_t best = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        if (e > best)
            best = e;
    }
    return best;
}
