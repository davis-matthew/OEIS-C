/*
 * A056789  a(n) = Sum_{k=1..n} lcm(k,n)/gcd(k,n).
 * https://oeis.org/A056789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_seq_u128, pow_seq_u128, rec_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056789
   Formula: a(n) = sum_{k=1..n} lcm(k,n)/gcd(k,n)
   Equivalent: a(n) = 1 + (1/2) * sum_{d|n, d>1} φ(d³)          (OEIS)
   where φ is Euler's totient function.
   Code uses only integer arithmetic and works for any unsigned
   seq_t and idx_t defined by the build. */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for fixed width integer types */
#include "../utils/mul_seq_u128.h"
#include "../utils/pow_seq_u128.h"
#include "../utils/rec_sum.h"

/* ------------------------------------------------------------------ */
/* Internal helpers                                                      */
/* ------------------------------------------------------------------ */

/* Factorise n into prime‑power arrays.                                 */
static void factorize(idx_t n, idx_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[(*cnt)++] = p;
            exps[ (*cnt) - 1] = e;
        }
    }
    if (n > 1)        /* remaining prime factor */
    {
        primes[(*cnt)++] = n;
        exps[ (*cnt) - 1] = 1;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* safety – OEIS offset is 1 */
    if (n == 1) return (seq_t)1;/* a(1) = 1 */

    /* Factorise n */
    idx_t primes[64];
    int   exps[64];
    size_t pcnt;
    factorize(n, primes, exps, &pcnt);

    /* Sum over all divisors d > 1 of φ(d³)/2.  accumulates in 128‑bit */
    unsigned __int128 sum128 = 0;
    rec_sum(0, pcnt, primes, exps, (seq_t)1, (seq_t)1, &sum128);

    /* Final result: a(n) = 1 + sum */
    unsigned __int128 result128 = 1 + sum128;
    return (seq_t)result128;
}
