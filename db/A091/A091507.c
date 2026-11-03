/*
 * A091507  Product of the anti-divisors of n.
 * https://oeis.org/A091507
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the distinct quotients s/d, where s ∈ {2n−1,
 *            2n, 2n+1} and d runs over all odd divisors >1 of s
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 93, u64 n <= 246,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 791 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition of A091507. */

#include <stdbool.h>
#include <stddef.h>

/* add val to uniq[] if absent; uniq_len holds current length */
static inline void add_unique(seq_t val, seq_t uniq[], size_t *uniq_len)
{
    for (size_t i = 0; i < *uniq_len; ++i)
        if (uniq[i] == val)
            return;                 /* already present */
    uniq[(*uniq_len)++] = val;
}

/* factorization of s, generate its odd divisors >1 and store the
   corresponding quotients s/d in uniq[]. */
static inline void process_s(idx_t s, seq_t uniq[], size_t *uniq_len)
{
    /* strip powers of 2 – divisors must be odd */
    idx_t temp = s;
    while ((temp & 1U) == 0)          /* while divisible by 2 */
        temp >>= 1;

    /* collect odd prime factors */
    idx_t  primes[16];
    int    exps[16];
    size_t pcnt = 0;

    idx_t p = 3;
    while (p * p <= temp) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
        p += 2;
    }
    if (temp > 1) {                   /* remaining prime factor > sqrt */
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* generate all odd divisors via back‑tracking pruning */
    idx_t divs[512];
    size_t divcnt = 1;
    divs[0] = 1;                      /* divisor 1 */

    for (size_t i = 0; i < pcnt; ++i) {
        idx_t prime = primes[i];
        int   exp   = exps[i];
        size_t oldcnt = divcnt;
        idx_t pow = 1;
        for (int e = 1; e <= exp; ++e) {
            pow *= prime;
            for (size_t j = 0; j < oldcnt; ++j)
                divs[divcnt++] = divs[j] * pow;
        }
    }

    /* collect quotients for odd divisors >1 */
    for (size_t i = 1; i < divcnt; ++i) {    /* skip divisor 1 */
        idx_t d = divs[i];
        if (d <= 1)
            continue;
        idx_t q = s / d;                    /* quotient >1 */
        if (q > 1)
            add_unique((seq_t)q, uniq, uniq_len);
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* offset behaviour: a(3) is first term; below offset undefined */
    if (n < 3)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    seq_t uniq[1024];
    size_t uniq_len = 0;

    /* process s = 2n-1, 2n, 2n+1 */
    for (int off = -1; off <= 1; ++off) {
        idx_t s = 2 * n + (idx_t)off;
        process_s(s, uniq, &uniq_len);
    }

    for (size_t i = 0; i < uniq_len; ++i)
        prod *= uniq[i];

    return prod;
}
