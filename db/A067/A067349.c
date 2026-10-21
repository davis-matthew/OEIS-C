/*
 * A067349  Number of divisors of sigma(n)+phi(n).
 * https://oeis.org/A067349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A000010(n) + A000203(n))
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _divisor_count_u64, build_primes_upto, floor_sqrt, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors of sigma(n) + phi(n) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/_divisor_count_u64.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Generic divisor count for arbitrary-precision seq_t values */
static seq_t count_divisors_generic(seq_t x)
{
    if (x <= 1) return 1;
    seq_t sqrt_x = floor_sqrt(x);
    idx_t limit = (idx_t)sqrt_x;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t count = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* stop if p^2 > current x */
        if ((seq_t)p * (seq_t)p > x) break;
        unsigned exp = 0;
        while (x % (seq_t)p == 0) {
            x /= (seq_t)p;
            ++exp;
        }
        if (exp) count *= (seq_t)(exp + 1);
    }
    if (x > 1) count *= 2;      /* remaining prime factor */
    free(primes);
    return count;
}

seq_t a(idx_t n)
{
    seq_t sigma_val = sigma_of(n);
    seq_t phi_val   = phi(n);
    seq_t sum = sigma_val + phi_val;

    if ((seq_t)UINT64_MAX >= sum) {          /* sum fits into uint64_t */
        uint64_t m = (uint64_t)sum;
        uint64_t d = _divisor_count_u64(m);
        return (seq_t)d;
    }
    return count_divisors_generic(sum);
}
