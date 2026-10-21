/*
 * A074251  Sum of the squarefree numbers from the smallest prime factor of n
 *          to the largest prime factor of n.
 * https://oeis.org/A074251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of square‑free integers k with spf(n) ≤ k ≤ lpf(n)
 *            (spf = smallest prime factor, lpf = largest prime factor)
 * Range:     u8 n <= 61, u16 n <= 925, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t *spf = NULL;      /* smallest prime factor table */
static size_t  max_n = 0;      /* maximum index that has been sieved */

/* Rebuild the smallest‑prime‑factor table up to exactly N (inclusive). */
static void ensure_up_to(idx_t N)
{
    if (N <= (idx_t)max_n)
        return;

    size_t newmax = (size_t)N;
    spf = (idx_t *)realloc(spf, (newmax + 1) * sizeof(idx_t));

    /* initialise the whole table – recomputed from scratch for simplicity */
    for (size_t i = 0; i <= newmax; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= (idx_t)newmax; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            if ((idx_t)((unsigned long long)i * i) <= (idx_t)newmax) {
                for (idx_t j = i * i; j <= (idx_t)newmax; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;            /* record smallest prime factor */
            }
        }
    }
    max_n = newmax;
}

/* Determine if x is square‑free using the spf table. */
static bool squarefree(idx_t x)
{
    if (x < 2)
        return true;          /* 1 is square‑free by convention */

    while (x > 1) {
        idx_t p = spf[x];
        if (p == 0)          /* x itself is a prime > current sieve range? */
            p = x;
        int cnt = 0;
        while (x % p == 0) {
            ++cnt;
            if (cnt >= 2)    /* repeated prime factor → not square‑free */
                return false;
            x /= p;
        }
    }
    return true;
}

/* Compute the largest prime factor of n. */
static idx_t largest_prime_factor(idx_t n)
{
    idx_t temp = n;
    idx_t lp = 0;
    while (temp > 1) {
        idx_t p = spf[temp];
        if (p == 0)
            p = temp;        /* n is prime > current sieve range */
        lp = p;
        while (temp % p == 0)
            temp /= p;
    }
    return lp;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;         /* offset 1: a(1)=0 */

    ensure_up_to(n);

    idx_t sp = spf[n];
    if (sp == 0)                  /* n is a prime larger than the sieve range */
        sp = n;

    idx_t lp = largest_prime_factor(n);

    seq_t sum = 0;
    for (idx_t x = sp; x <= lp; ++x)
        if (squarefree(x))
            sum += (seq_t)x;

    return sum;
}
