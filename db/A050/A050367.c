/*
 * A050367  Number of ways to factor n into 2 kinds of 2, 3 kinds of 3, ...
 * https://oeis.org/A050367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of ways to factor n into products of integers ≥ 2,
 *            where each factor m appears with multiplicity k and contributes
 *            a weight C(m + k - 1, k). The Dirichlet g.f. is ∏_{m≥2} (1 -
 *            m^{-s})^{-m}
 * Range:     u8 n <= 35, u16 n <= 959, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find binomial coefficients */
#include "../utils/binom.h"
#include <stdlib.h>

/* Simple linked‑list cache for (n, minBase) → value */
typedef struct CacheEntry {
    idx_t n;
    idx_t minBase;
    seq_t value;
    struct CacheEntry *next;
} CacheEntry;

static CacheEntry *cacheHead = NULL;

/* Recursively compute a(n, minBase) – factor base values must be strictly
   increasing to avoid duplicate counts. */
static seq_t compute_at(idx_t n, idx_t minBase)
{
    if (n == 1)                     /* base case: the empty product */
        return (seq_t)1;
    if (minBase > n)                /* no base can be chosen */
        return (seq_t)0;

    /* Check cache */
    for (CacheEntry *p = cacheHead; p; p = p->next) {
        if (p->n == n && p->minBase == minBase)
            return p->value;
    }

    seq_t sum = (seq_t)0;

    /* Enumerate divisors from minBase up to sqrt(n) */
    for (idx_t d = minBase; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* First divisor d */
        {
            idx_t rem = n;
            idx_t exp = 0;
            while (rem % d == 0) {
                ++exp;
                rem /= d;
                seq_t weight = binom(d + exp - 1, exp);
                seq_t sub = compute_at(rem, d + 1);
                sum += weight * sub;
            }
        }

        /* Second divisor n/d, if distinct */
        idx_t od = n / d;
        if (od != d && od >= minBase) {
            idx_t rem = n;
            idx_t exp = 0;
            while (rem % od == 0) {
                ++exp;
                rem /= od;
                seq_t weight = binom(od + exp - 1, exp);
                seq_t sub = compute_at(rem, od + 1);
                sum += weight * sub;
            }
        }
    }

    /* Handle the base n itself (e = 1) */
    if (minBase <= n) {
        seq_t weight = binom(n, 1);          /* C(n,1) = n */
        sum += weight * compute_at(1, n + 1); /* remainder is 1 */
    }

    /* Store in cache */
    CacheEntry *c = (CacheEntry *)malloc(sizeof(CacheEntry));
    c->n = n;
    c->minBase = minBase;
    c->value = sum;
    c->next = cacheHead;
    cacheHead = c;

    return sum;
}

/* Public entry point required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset does not matter for n < 1 */
    return compute_at(n, 2);
}
