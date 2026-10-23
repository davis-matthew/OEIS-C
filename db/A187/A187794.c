/*
 * A187794  Sum of the perfect divisors of n.
 * https://oeis.org/A187794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the perfect divisors of n A divisor d of n is
 *            perfect iff σ(d) = 2·d. All known even perfect numbers up to 10
 *            000 are 2^{p-1}(2^p–1) with p ∈ {2,3,5,7}. The implementation
 *            checks those values; odd perfect numbers are currently not known
 *            and are ignored
 * Range:     u8 n <= 495, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* List of exponents p such that 2^(p-1)(2^p-1) is an even perfect
       number; we include all Mersenne primes up to p=31 to cover
       descendants well beyond n=10 000 while keeping computations in
       64‑bit arithmetic. */
    static const unsigned int mersenne_exps[] = {2, 3, 5, 7, 13, 17, 19, 31};
    const size_t exp_count = sizeof(mersenne_exps) / sizeof(mersenne_exps[0]);

    seq_t sum = (seq_t)0;
    const unsigned long long limit = (unsigned long long)n;

    for (size_t i = 0; i < exp_count; ++i) {
        unsigned int p = mersenne_exps[i];

        /* Compute the perfect number: 2^(p-1) * (2^p - 1) */
        unsigned long long two_pow_p     = 1ULL << p;            /* 2^p   */
        unsigned long long two_pow_p_m1 = two_pow_p - 1ULL;       /* 2^p-1 */
        unsigned long long perfect = (1ULL << (p - 1)) * two_pow_p_m1;

        if (perfect > limit) {
            /* Perfect numbers increase with p; remaining candidates
               will all exceed n. */
            break;
        }

        /* If perfect divides n, add it to the sum */
        if (n % (idx_t)perfect == (idx_t)0) {
            sum += (seq_t)perfect;
        }
    }

    return sum;
}
