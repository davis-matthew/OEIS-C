/*
 * A048105  Number of non-unitary divisors of n.
 * https://oeis.org/A048105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) - 2^omega(n) (number of non‑unitary divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* idx_t and seq_t are defined by the build environment.  */

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m == 0)               /* offset is 1; behaviour for n = 0 is irrelevant */
        return (seq_t)0;

    /* Containers for distinct prime factors and their exponents. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;

    /* Handle factor 2 separately. */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            e++;
        }
        primes[pcnt] = 2;
        exps[pcnt]  = e;
        pcnt++;
    }

    /* Trial division by odd numbers up to sqrt(m). */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt]  = e;
            pcnt++;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt]  = 1;
        pcnt++;
    }

    /* Compute d(n) = product (e_i+1) and 2^omega(n). */
    seq_t d   = 1;
    seq_t pow2 = 1;
    for (size_t i = 0; i < pcnt; i++) {
        d   *= (seq_t)(exps[i] + 1);
        pow2 *= 2;
    }

    return d - pow2;
}
