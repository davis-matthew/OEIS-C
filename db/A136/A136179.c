/*
 * A136179  a(n) = Product_{k=1..d(n)-1} gcd(b(k), b(k+1)), where b(k) is the
 *          k-th positive divisor of n and d(n) is the number of positive
 *          divisors of n.
 * https://oeis.org/A136179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{d(n)-1} gcd(b_k , b_{k+1})
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 215, u64 n <= 719,
 *            u128 n <= 796, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* b_k is the k‑th positive divisor of n in increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static int cmp_seq(const void *a, const void *b)
{
    seq_t const va = *(const seq_t *)a;
    seq_t const vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* For n == 1 only divisor is 1 → empty product = 1. */
    if (n == 1) return (seq_t)1;

    /* ---------- factor n by trial division ---------- */
    uint64_t rem = (uint64_t)n;
    uint64_t primes[64];
    uint8_t   exps[64];
    size_t pf_cnt = 0;

    for (uint64_t p = 2; p <= rem / p; ++p)
    {
        if (rem % p == 0)
        {
            uint8_t e = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt]   = e;
            ++pf_cnt;
        }
    }
    if (rem > 1)
    {
        primes[pf_cnt] = rem;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* ---------- number of divisors d(n) ---------- */
    size_t dcount = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        dcount *= (size_t)(exps[i] + 1);

    /* ---------- generate all divisors ---------- */
    seq_t *divs = (seq_t *)malloc(dcount * sizeof(seq_t));
    if (!divs) return (seq_t)1;               /* unlikely */

    size_t sz = 1;
    divs[0] = (seq_t)1;                       /* the divisor 1 */

    for (size_t i = 0; i < pf_cnt; ++i)
    {
        uint64_t p = primes[i];
        /* powers of the current prime */
        seq_t pow[65];          /* exps[i] is small for all practical n */
        pow[0] = (seq_t)1;
        for (unsigned e = 1; e <= exps[i]; ++e)
            pow[e] = pow[e-1] * (seq_t)p;

        size_t old_sz = sz;
        for (size_t j = 0; j < old_sz; ++j)
        {
            seq_t base = divs[j];
            for (unsigned e = 1; e <= exps[i]; ++e)
                divs[sz++] = base * pow[e];
        }
    }

    /* ---------- sort divisors ---------- */
    qsort(divs, dcount, sizeof(seq_t), cmp_seq);

    /* ---------- product of consecutive gcds ---------- */
    seq_t result = (seq_t)1;
    for (size_t i = 1; i < dcount; ++i)
        result *= gcd(divs[i-1], divs[i]);

    free(divs);
    return result;
}
