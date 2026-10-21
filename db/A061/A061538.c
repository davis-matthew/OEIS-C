/*
 * A061538  Product of all divisors of n, divided by product of unitary
 *          divisors; or equivalently product of non-unitary divisors of n.
 * https://oeis.org/A061538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n ^ ((tau(n) - 2^k) / 2), where tau(n) = product (e_i+1)
 *            and k = number of distinct prime factors of n. For n a square,
 *            the exponent is half-integer and the result equals
 *            n^((tau-udiv)/2) = n^q * sqrt(n) with q = (tau-udiv-1)/2
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 143, u64 n <= 359,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, floor_sqrt, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    // handle n = 1
    if (n == 1)
        return (seq_t)1;

    /* Factor n (up to 64 distinct primes, 10k has at most 6). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute divisor count τ(n) = Π (e_i + 1). */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* Count of unitary divisors = 2^k. */
    seq_t unitary = (seq_t)1 << cnt;

    /* Difference diff = τ(n) - 2^k.  diff ≥ 0. */
    seq_t diff = tau - unitary;
    uint64_t diff64 = (uint64_t)diff;   // safe for n ≤ 10k

    if (diff64 == 0)
        return (seq_t)1;   // no non-unitary divisors

    /* If diff even → integer exponent; otherwise sqrt needed. */
    if ((diff64 & 1ULL) == 0) {            // even
        idx_t exp = (idx_t)(diff64 / 2);
        return seq_pow((seq_t)n, exp);
    } else {                              // odd (n is a perfect square)
        idx_t q   = (idx_t)(diff64 / 2);
        seq_t pow = seq_pow((seq_t)n, q);
        seq_t sqrt_n = floor_sqrt((seq_t)n);  // n is square
        return seq_mul(pow, sqrt_n);
    }
}
