/*
 * A068237  Numerators of arithmetic derivative of 1/n: -A003415(n)/n^2.
 * https://oeis.org/A068237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, look
 * Formula:   a(n) = -A003415(n)/n^2 (numerator of the reduced fraction)
 * Range:     i8 n <= 169, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Prime sieve up to a fixed limit.  Adjust MAX_PRIME if larger primes are needed. */
#define MAX_PRIME 1000000
static uint64_t primes[80000];
static int prime_count = 0;

/* Initialize the prime array exactly once. */
static void init_primes(void)
{
    if (prime_count > 0)
        return;
    static unsigned char is_comp[MAX_PRIME + 1];
    memset(is_comp, 0, sizeof(is_comp));
    for (int i = 2; i * i <= MAX_PRIME; ++i)
    {
        if (!is_comp[i])
        {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_comp[j] = 1;
        }
    }
    for (int i = 2; i <= MAX_PRIME; ++i)
    {
        if (!is_comp[i])
            primes[prime_count++] = (uint64_t)i;
    }
}

/* Euclidean gcd for __int128 values.  Both arguments are non‑negative. */
static __int128 gcd128(__int128 a, __int128 b)
{
    while (b != 0)
    {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a < 0 ? -a : a;
}

/* Compute A068237(n), defined as the numerator of the reduced fraction
   -A003415(n) / n^2, where A003415(n) is the arithmetic derivative of n. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    init_primes();

    uint64_t nn = (uint64_t)n;
    uint64_t temp = nn;
    __int128 deriv = 0;

    /* Factor n using the pre‑computed primes. */
    int idx = 0;
    while (idx < prime_count && primes[idx] * primes[idx] <= temp)
    {
        uint64_t p = primes[idx];
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            deriv += (__int128)(nn / p) * exp;
        }
        ++idx;
    }

    /* Remaining factor, if any, is prime. */
    if (temp > 1)
    {
        deriv += (__int128)(nn / temp);
    }

    /* deriv now holds A003415(n).  We need –deriv / n^2, reduced. */
    __int128 denom = (__int128)nn * (__int128)nn;
    __int128 g = gcd128(deriv, denom);
    __int128 reduced_num = -(deriv / g);   /* negative numerator */

    return (seq_t)reduced_num;
}
