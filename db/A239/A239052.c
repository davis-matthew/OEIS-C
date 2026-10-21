/*
 * A239052  Sum of divisors of 4*n-2.
 * https://oeis.org/A239052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of divisors of 4*n-2
 * Range:     u8 n <= 31, u16 n <= 5197, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute σ(m) for odd m: product over prime powers of sum of powers */
static inline seq_t sigma_odd(uint64_t m)
{
    if (m <= 1U)
        return (seq_t)1;          /* σ(1) = 1 */

    seq_t result = (seq_t)1;
    uint64_t num = m;
    uint64_t d = 3;                /* start with first odd prime */

    /* trial division up to sqrt(num) */
    while (d * d <= num) {
        if (num % d == 0) {
            int e = 0;
            while (num % d == 0) {
                num /= d;
                ++e;
            }

            /* sum of the powers of d up to d^e */
            seq_t sum = (seq_t)1;
            seq_t power = (seq_t)1;
            for (int i = 1; i <= e; ++i) {
                power *= (seq_t)d;
                sum += power;
            }
            result *= sum;
        }
        d += 2;                    /* only odd divisors needed */
    }

    /* if a prime factor remains > sqrt(original m) */
    if (num > 1U)
        result *= ((seq_t)1 + (seq_t)num);  /* σ(p) = 1 + p */

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset not used for n < 1 */

    uint64_t odd = 2ULL * (uint64_t)n - 1ULL;   /* 2n-1 is odd */
    seq_t sigma = sigma_odd(odd);
    return (seq_t)3 * sigma;               /* σ(4n-2) = 3 * σ(2n-1) */
}
