/*
 * A065968  a(n) = n * Sum_{primes p dividing n} (1 - 1/p).
 * https://oeis.org/A065968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Σ_{p|n} (1 - 1/p) * Computation: a(n) = Σ_{p|n} (n/p)
 *            * (p-1) * The function factors n using a precomputed
 *            smallest‑prime‑factor table * (size 200000) which is inexpensive
 *            to build and fast to use for all * calls. The result is exact
 *            for any n for which the table can factor * the numbe...
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 200000           /* enough for n ≤ 10000, and safe a bit larger */
static int spf[MAX_N + 1];    /* smallest prime factor table */
static int primes[20000];     /* list of primes up to MAX_N */
static int prime_cnt = 0;     /* number of primes stored */
static int initialized = 0;   /* flag for one‑time table construction */

static void init_spf(void)
{
    int i, j;
    for (i = 0; i <= MAX_N; ++i) spf[i] = i;
    for (i = 2; i * i <= MAX_N; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (j = i * i; j <= MAX_N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    for (i = 2; i <= MAX_N; ++i)
        if (spf[i] == i) primes[prime_cnt++] = i;
    initialized = 1;
}

seq_t a(idx_t n)
{
    /* Lazy initialization of the factor table.  This code is executed
     * the first time a() is called, which is fine because the first
     * call will be for a reasonably small n (n ≥ 1). */
    if (!initialized) init_spf();

    if (n <= 1) return 0;          /* a(1) = 0 by definition */

    idx_t temp = n;
    seq_t result = 0;

    /* Iterate over distinct prime factors using the spf table. */
    while (temp > 1) {
        idx_t p = spf[temp];        /* a prime divisor of temp */
        /* Add the term (n/p) * (p-1).  n/p is an integer. */
        result += (seq_t)(n / p) * (p - 1);

        /* Skip all occurrences of p to keep distinct primes only. */
        while (temp % p == 0) temp /= p;
    }

    return result;
}
