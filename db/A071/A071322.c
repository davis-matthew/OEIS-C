/*
 * A071322  Alternating sum of all prime factors of n; primes nonincreasing,
 *          starting with the largest prime factor: A006530(n).
 * https://oeis.org/A071322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{k} (-1)^{i+1} p_i, where p_1 >= p_2 >= … >=
 *            p_k are the prime factors of n (with multiplicity)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute alternating sum of prime factors of n.
   Results are always non‑negative for n >= 1. */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return (seq_t)0;

    /* prime factors will not exceed 64, so a statically‑allocated array is safe */
    uint64_t factors[64];
    int fcnt = 0;

    uint64_t t = (uint64_t)n;

    for (uint64_t p = 2; p * p <= t; ++p) {
        while (t % p == 0) {
            if (fcnt >= (int)sizeof(factors)/sizeof(factors[0]))
                /* safety: should never happen for reasonable n */
                return (seq_t)0;
            factors[fcnt++] = p;
            t /= p;
        }
    }
    if (t > 1) {          /* last prime factor > sqrt(original t) */
        if (fcnt >= (int)sizeof(factors)/sizeof(factors[0]))
            return (seq_t)0;
        factors[fcnt++] = t;
    }

    seq_t sum = 0;
    int sign = 1;         /* +1 for first prime, then alternating */

    for (int i = fcnt - 1; i >= 0; --i) {
        if (sign > 0)
            sum += (seq_t)factors[i];
        else
            sum -= (seq_t)factors[i];
        sign = -sign;
    }

    return sum;
}
