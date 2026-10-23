/*
 * A133690  Expansion of (phi(-q) * phi(q^2))^2 in powers of q where phi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A133690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 2939, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative formula for A133690
   a(0) = 1
   For n ≥ 1 :  a(n) = –4 * b(n)
   b is multiplicative with
       b(2)   = –2
       b(2^e) = –6   for e > 1
       b(p^e) = 1 + p + p^2 + … + p^e   for odd prime p
*/
#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation for seq_t (wrapper for simple power calculation) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur    = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* a(0) defined as 1 */

    /* factorise n and accumulate multiplicative value b(n) */
    seq_t bn = 1;
    idx_t temp = n;

    /* handle factor 2 separately */
    if (temp % 2 == 0)
    {
        idx_t cnt = 0;
        while (temp % 2 == 0)
        {
            temp /= 2;
            cnt++;
        }
        seq_t term = (cnt == 1) ? (seq_t)(-2) : (seq_t)(-6);
        bn *= term;
    }

    /* trial division for odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            idx_t cnt = 0;
            while (temp % p == 0)
            {
                temp /= p;
                cnt++;
            }
            /* compute 1 + p + p^2 + ... + p^cnt */
            seq_t term = 1;          /* p^0 */
            seq_t powp = 1;
            for (idx_t i = 1; i <= cnt; ++i)
            {
                powp *= (seq_t)p;    /* p^i */
                term += powp;
            }
            bn *= term;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1)
    {
        /* temp is a prime, exponent = 1 */
        seq_t term = 1 + (seq_t)temp;   /* 1 + p */
        bn *= term;
    }

    /* a(n) = -4 * b(n) */
    return - (seq_t)4 * bn;
}
