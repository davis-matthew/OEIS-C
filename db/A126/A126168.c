/*
 * A126168  Sum of the proper infinitary divisors of n.
 * https://oeis.org/A126168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma_inf(n) - n * sigma_inf(n) = product over p^e||n of
 *            product over set bits a of (1 + p^(2^a)) * a(1) = 0
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* sum of proper infinitary divisors of 1 is 0 */

    seq_t sigma = (seq_t)1;
    uint64_t temp = n;

    /* factor n by trial division (fast enough for n ≤ 10000) */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if ((temp % p) != 0) continue;
        uint64_t exp = 0;
        while ((temp % p) == 0) { temp /= p; ++exp; }

        /* term for this prime power */
        seq_t term = (seq_t)1;
        uint64_t mask = exp;
        int a_bit = 0;
        while (mask) {
            if (mask & 1ULL) {
                seq_t pow_val = seq_pow((seq_t)p, (idx_t)(1ULL << a_bit)); /* p^(2^a) */
                term *= (pow_val + (seq_t)1);
            }
            mask >>= 1;
            ++a_bit;
        }
        sigma *= term;
    }

    /* remaining prime factor (if any) has exponent 1 */
    if (temp > 1) {
        seq_t term = (seq_t)temp + (seq_t)1;
        sigma *= term;
    }

    return sigma - (seq_t)n;
}
