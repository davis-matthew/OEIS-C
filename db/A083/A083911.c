/*
 * A083911  Number of divisors of n that are congruent to 1 modulo 10.
 * https://oeis.org/A083911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that d ≡ 1 (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division, enumerates all its
   divisors recursively, and counts those congruent to 1 modulo 10. */

#include <stddef.h>

static void enumerate_divisors(int idx, idx_t curr,
                               const int *primes, const int *exps,
                               int pf_cnt, seq_t *count)
{
    if (idx == pf_cnt) {
        if (curr % 10 == 1)
            (*count)++;
        return;
    }
    idx_t p = primes[idx];
    idx_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(idx + 1, curr * mult,
                           primes, exps, pf_cnt, count);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    /* Prime factorisation of n, storing primes and exponents. */
    int primes[64];
    int exps[64];
    int pf_cnt = 0;

    idx_t m = n;
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            e++;
        }
        primes[pf_cnt] = 2;
        exps[pf_cnt++] = e;
    }

    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            primes[pf_cnt] = (int)p;
            exps[pf_cnt++] = e;
        }
    }

    if (m > 1) { /* remaining prime factor */
        primes[pf_cnt] = (int)m;
        exps[pf_cnt++] = 1;
    }

    seq_t count = 0;
    enumerate_divisors(0, 1, primes, exps, pf_cnt, &count);
    return count;
}
