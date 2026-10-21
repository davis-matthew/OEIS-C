/*
 * A069184  Sum of divisors d of n such that d or n/d is odd.
 * https://oeis.org/A069184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = Π_{p^e || n} φ(p^e) where φ(2^e) = 2^e + 1 * and φ(p^e) =
 *            (p^(e+1)-1)/(p-1) for odd primes p. This is the * sum of
 *            divisors d of n such that d or n/d is odd
 * Range:     u8 n <= 16, u16 n <= 3786, u32 n <= 7545, u64 n <= 7545,
 *            u128 n <= 7545, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_local(seq_t base, int exp)
{
    seq_t res = 1;
    while (exp > 0) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N == 0) return 0;   /* n>=1 by definition, safe guard */
    if (N == 1) return 1;

    seq_t result = 1;
    uint64_t m = N;

    /* factor out primes up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (p == 2) {
            seq_t two_power = pow_seq_local((seq_t)2, e);
            result *= (two_power + (seq_t)1);
        } else {
            seq_t power = pow_seq_local((seq_t)p, e + 1);
            seq_t numerator = power - (seq_t)1;
            seq_t denom = (seq_t)(p - 1);
            result *= (numerator / denom);
        }
    }

    /* remaining prime factor (if any) has exponent 1 */
    if (m > 1) {
        uint64_t p = m;   /* prime > sqrt(original N) */
        if (p == 2) {
            seq_t two_power = 2;          /* 2^1 */
            result *= (two_power + (seq_t)1);
        } else {
            seq_t power = (seq_t)p * (seq_t)p; /* p^(1+1) */
            seq_t numerator = power - (seq_t)1;
            seq_t denom = (seq_t)(p - 1);
            result *= (numerator / denom);
        }
    }

    return result;
}
