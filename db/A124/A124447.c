/*
 * A124447  a(n) = lcm(A066840(n), A124440(n)).
 * https://oeis.org/A124447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( sum of coprime integers <= floor(n/2), * sum of
 *            coprime integers > floor(n/2) )
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;  /* a(1) = 0 */

    seq_t n_seq = (seq_t)n;
    /* Distinct prime factors of n */
    uint64_t primes[64];
    size_t pcnt = 0;
    uint64_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pcnt++] = tmp;

    /* Euler totient φ(n) */
    seq_t phi = n_seq;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        phi = phi / p * (p - (seq_t)1);
    }

    /* Sum of all coprime integers < n (n is never coprime to itself for n>1) */
    seq_t S_total = (n_seq * phi) / (seq_t)2;

    idx_t m = n / 2;          /* floor(n/2) */
    seq_t S1 = 0;
    if (m > 0) {
        size_t subsets = (size_t)1 << pcnt;  /* 2^pcnt subsets */
        for (size_t mask = 0; mask < subsets; ++mask) {
            seq_t d = (seq_t)1;
            int bits = 0;
            for (size_t i = 0; i < pcnt; ++i) {
                if (mask & ((size_t)1 << i)) {
                    d = d * (seq_t)primes[i];
                    bits++;
                }
            }
            idx_t t = m / (uint64_t)d;
            if (t == 0)
                continue;
            seq_t T = (seq_t)t * (seq_t)(t + 1) / (seq_t)2;  /* sum 1..t */
            seq_t term = d * T;
            if (bits % 2 == 0)   /* μ(d) = +1 */
                S1 += term;
            else                 /* μ(d) = -1 */
                S1 -= term;
        }
    }

    seq_t S2 = S_total - S1;

    /* Handle cases where one sum is zero: lcm(x,0)=x, lcm(0,0)=0 */
    if (S1 == 0 || S2 == 0)
        return S1 + S2;

    seq_t g = gcd(S1, S2);
    return (S1 / g) * S2;
}
