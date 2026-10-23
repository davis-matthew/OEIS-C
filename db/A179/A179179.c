/*
 * A179179  a(n) = phi(n) - omega(n) = A000010(n) - A001221(n).
 * https://oeis.org/A179179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) – ω(n) (Euler’s totient minus number of distinct
 *            primes)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    uint64_t tmp = n;      /* work on a temporary 64‑bit integer */
    seq_t phi   = (seq_t)tmp;  /* φ(n) starts as n */
    seq_t omega = 0;            /* number of distinct prime factors */

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            /* p is a distinct prime factor */
            ++omega;
            phi -= phi / (seq_t)p;   /* φ *= (1 – 1/p) */

            /* remove all powers of p from tmp */
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* If tmp > 1, it is a remaining prime factor. */
    if (tmp > 1)
    {
        ++omega;
        phi -= phi / (seq_t)tmp;
    }

    return phi - omega;
}
