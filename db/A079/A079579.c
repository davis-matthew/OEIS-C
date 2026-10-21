/*
 * A079579  Totally multiplicative with p -> (p-1)*p, p prime.
 * https://oeis.org/A079579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} p^e*(p-1)^e , a(1)=1 (totally multiplicative
 *            with p→(p-1)*p)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t pe      = pow_seq_t((seq_t)p,     exp);
            seq_t p_minus = pow_seq_t((seq_t)(p - 1), exp);
            result *= pe;
            result *= p_minus;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        seq_t pe      = (seq_t)temp;
        seq_t p_minus = (seq_t)(temp - 1);
        result *= pe;
        result *= p_minus;
    }

    return result;
}
