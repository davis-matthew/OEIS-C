/*
 * A053446  Multiplicative order of 3 mod m, where gcd(m, 3) = 1.
 * https://oeis.org/A053446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 3 modulo floor((3*n-1)/2)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

static void factorize(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            pr[c] = d;
            ex[c] = 0;
            while (n % d == 0) {
                n /= d;
                ex[c]++;
            }
            c++;
        }
        d = (d == 2) ? 3 : d + 2;          /* skip even numbers after 2 */
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        c++;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for offset < 1 */

    /* Compute m = floor((3*n-1)/2) */
    uint64_t m = (3ULL * n - 1ULL) / 2ULL;
    if (m <= 1) return 1;                /* order modulo 1 is defined as 1 */

    /* Factorize m */
    uint64_t primes1[20];
    int exps1[20];
    size_t cnt1;
    factorize(m, primes1, exps1, &cnt1);

    /* Compute φ(m) */
    uint64_t phi = m;
    for (size_t i = 0; i < cnt1; ++i)
        phi -= phi / primes1[i];

    /* Factorize φ(m) */
    uint64_t primes2[20];
    int exps2[20];
    size_t cnt2;
    factorize(phi, primes2, exps2, &cnt2);

    /* Find the multiplicative order of 3 modulo m */
    uint64_t order = phi;
    for (size_t i = 0; i < cnt2; ++i) {
        uint64_t q = primes2[i];
        while (order % q == 0) {
            if (mod_pow(3, order / q, (seq_t)m) == 1)
                order /= q;
            else
                break;
        }
    }

    return (seq_t)order;
}
