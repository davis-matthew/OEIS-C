/*
 * A178450  Dirichlet inverse of A034448 (unitary sigma).
 * https://oeis.org/A178450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(1) = 1; for prime powers a(p^e) = 2*p^(e/2) if e is even,
 *            -(p+1)*p^((e-1)/2) if e is odd. The sequence is multiplicative
 * Range:     i8 n <= 65, i16 n <= 9976, i32 n <= 9976, i64 n <= 9976,
 *            i128 n <= 9976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* fast exponentiation for seq_t base */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* value of the sequence at a prime power p^e */
static seq_t a_prime_power(uint64_t p, int e)
{
    if ((e & 1) == 0) /* e even */
    {
        seq_t pow = pow_uint((seq_t)p, e >> 1);
        return 2 * pow;
    }
    else /* e odd */
    {
        seq_t pow = pow_uint((seq_t)p, (e - 1) >> 1);
        seq_t val = (seq_t)(p + 1) * pow;
        return -val;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t tmp = n;
    seq_t result = 1;

    /* trial division factorization */
    for (idx_t p = 2; (uint64_t)p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            result *= a_prime_power(p, e);
        }
    }

    if (tmp > 1) /* remaining prime factor */
    {
        result *= a_prime_power(tmp, 1);
    }

    return result;
}
