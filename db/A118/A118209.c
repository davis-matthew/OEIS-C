/*
 * A118209  Expansion of Sum_{k>=1} lambda(k) * k * x^k/(1 + x^k) where
 *          lambda(k) is the Liouville function, A008836.
 * https://oeis.org/A118209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} ((-1)^e * 2^(e+2) – 1) / 3 for p = 2, ( p *
 *            (−p)^e + 1 ) / (p + 1) for odd primes p
 * Range:     i8 n <= 31, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dependencies: seq_pow, seq_one */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

static seq_t prime_power_value(uint64_t p, int e)
{
    if (p == 2) {
        seq_t pow_val = seq_pow((seq_t)2, (idx_t)(e + 2));
        seq_t sign = (e & 1) ? (seq_t)(-1) : (seq_t)1;
        /* ((-1)^e * 2^(e+2) – 1) / 3 */
        return (sign * pow_val - (seq_t)1) / (seq_t)3;
    } else {
        seq_t pow_neg_p = seq_pow((seq_t)(- (int64_t)p), (idx_t)e);
        /* ( p * (−p)^e + 1 ) / (p + 1) */
        return ((seq_t)p * pow_neg_p + (seq_t)1) / (seq_t)(p + 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but safe */
    if (n == 1) return seq_one();

    uint64_t m = (uint64_t)n;
    seq_t result = seq_one();

    /* factor 2 separately */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        result *= prime_power_value(2, e);
    }

    /* odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= prime_power_value(p, e);
        }
    }

    /* remaining prime > sqrt(n) */
    if (m > 1) {
        result *= prime_power_value(m, 1);
    }

    return result;
}
