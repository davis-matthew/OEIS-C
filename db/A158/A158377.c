/*
 * A158377  a(1) = 0, a(n) = lcm(A034684(n), A034699(n)) for n >= 2.
 * https://oeis.org/A158377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(min prime‑power factor, max prime‑power factor)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   factor_u64, pow_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;      /* a(1) = 0 */

    uint64_t N = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(N, pr, ex, &cnt);      /* get prime factors and exponents */

    seq_t min_pow = 0, max_pow = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t pwr = pow_prime(pr[i], ex[i]);

        if (min_pow == 0 || pwr < min_pow) min_pow = pwr;
        if (max_pow == 0 || pwr > max_pow) max_pow = pwr;
    }

    /* prime powers from distinct primes are coprime.
       Only when n is a pure prime power do min_pow == max_pow. */
    if (min_pow == max_pow)
        return min_pow;
    else
        return min_pow * max_pow;
}
