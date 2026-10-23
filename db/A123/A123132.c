/*
 * A123132  Describe prime factorization of n (primes in ascending order and
 *          with repetition) (method A - initial term is 2).
 * https://oeis.org/A123132
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: For n ≥ 2, write the prime factorization of n in ascending prime order.
   For each prime p with exponent e, append the decimal digits of e followed by
   the decimal digits of p to form a base‑10 integer.  That integer is a(n). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute 10^e as a seq_t. */
static seq_t pow10_int(int e)
{
    seq_t r = 1;
    for (int i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* Count decimal digits of a positive seq_t. */
static int digit_count_seq(seq_t x)
{
    int cnt = 0;
    do {
        x /= 10;
        ++cnt;
    } while (x != 0);
    return cnt;
}

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 2. */
    if (n < 2)
        return 0;

    /* Factorize n. */
    uint64_t nn   = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt  = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t exponent = (seq_t)exps[i];
        seq_t prime    = (seq_t)primes[i];

        int d_exp   = digit_count_seq(exponent);
        result = result * pow10_int(d_exp) + exponent;

        int d_prime = digit_count_seq(prime);
        result = result * pow10_int(d_prime) + prime;
    }
    return result;
}
