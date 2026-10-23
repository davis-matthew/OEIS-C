/*
 * A167304  Totally multiplicative sequence with a(p) = 3*(p+2) for prime p.
 * https://oeis.org/A167304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e||n} (3*(p+2))^e * (totally multiplicative, a(p) =
 *            3*(p+2) for prime p)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply a seq_t by itself exp times.
 * Returns 1 if exp == 0. */
static seq_t pow_seq_local(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 1, behaviour for n < 1 is irrelevant (return 0) */
    if (n < 1)
        return 0;

    seq_t result = 1;
    uint64_t num = (uint64_t)n;

    /* trial division to factor num */
    for (uint64_t p = 2; p * p <= num; ++p)
    {
        if (num % p == 0)
        {
            int e = 0;
            while (num % p == 0)
            {
                num /= p;
                ++e;
            }
            seq_t base = (seq_t)3 * ((seq_t)p + 2);
            result *= pow_seq_local(base, e);
        }
    }

    /* if a prime factor remains */
    if (num > 1)
    {
        seq_t base = (seq_t)3 * ((seq_t)num + 2);
        result *= base;
    }

    return result;
}
