/*
 * A069783  a(n) = gcd(d(n!^3), d(n!)), where d() is the number of divisors
 *          function.
 * https://oeis.org/A069783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(d(n!^3), d(n!))
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 73, u64 n <= 185,
 *            u128 n <= 419, big any n
 * Helpers:   fact_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/fact_exp.h"

#define MAXN 10000             /* maximum n this implementation supports */

/* cached list of primes up to MAXN */
static int primes[MAXN / 8];
static int primeCnt = 0;
static bool primesInit = false;

/* initialise the prime list (once) */
static void init_primes(void)
{
    if (primesInit) return;
    bool sieve[MAXN + 1];
    for (int i = 0; i <= MAXN; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= MAXN; ++i)
        if (sieve[i])
            for (int j = i * i; j <= MAXN; j += i)
                sieve[j] = false;
    for (int i = 2; i <= MAXN; ++i)
        if (sieve[i]) primes[primeCnt++] = i;
    primesInit = true;
}

/* factorise x (x <= MAXN*MAXN) and add contributions to exp array */
static void add_factor(int x, int expArray[])
{
    int v = x;
    for (int i = 0; i < primeCnt; ++i) {
        int p = primes[i];
        if (p * p > v) break;
        if (v % p == 0) {
            int cnt = 0;
            while (v % p == 0) {
                v /= p;
                ++cnt;
            }
            expArray[p] += cnt;
        }
    }
    if (v > 1) expArray[v]++;   /* remaining prime factor */
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    init_primes();

    /* exponent tables for d(n!) and d(n!^3) */
    int expDn[MAXN + 1] = {0};
    int expDn3[MAXN + 1] = {0};

    /* compute prime exponents for each prime p <= n */
    for (int i = 0; i < primeCnt; ++i) {
        int p = primes[i];
        if (p > (int)n) break;

        idx_t e = fact_exp(n, p);
        /* d(n!) factor */
        add_factor((int)(e + 1), expDn);
        /* d(n!^3) factor */
        add_factor((int)(3 * e + 1), expDn3);
    }

    /* compute the gcd of the two factorizations */
    seq_t result = 1;
    for (int q = 2; q <= MAXN; ++q) {
        int g = expDn[q] < expDn3[q] ? expDn[q] : expDn3[q];
        if (g > 0) {
            for (int k = 0; k < g; ++k)
                result *= (seq_t)q;
        }
    }

    return result;
}
