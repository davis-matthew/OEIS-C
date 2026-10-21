/*
 * A061259  a(n)=Sum_{d|n} d*numbpart(d), where numbpart(d)=number of
 *          partitions of d, cf. A000041.
 * https://oeis.org/A061259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d * p(d) where p(d) = number of partitions of d
 *            (A000041)
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 88, u64 n <= 326,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Partition numbers cache ---------- */
static seq_t *p = NULL;          /* p[0..cap-1] */
static size_t p_cap = 0;         /* number of elements stored */

/* Compute partitions p[0..limit] using Euler's pentagonal theorem.
   This routine may be called with increasing limits. */
static void ensure_partitions(idx_t limit) {
    if (limit < (idx_t)p_cap)          /* already have enough */
        return;

    size_t old_cap = p_cap;            /* old size before realloc */
    size_t new_cap = (size_t)limit + 1;/* required size */

    seq_t *new_p = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_p) return;                /* allocation failure – silently abort */

    /* copy existing data */
    if (p) {
        for (size_t i = 0; i < old_cap; ++i)
            new_p[i] = p[i];
        free(p);
    }

    /* base case */
    if (old_cap == 0)
        new_p[0] = 1;                  /* p(0) = 1 */

    /* compute new partition values */
    for (size_t n = old_cap; n <= (size_t)limit; ++n) {
        if (n == 0) continue;          /* already set */
        seq_t pos = 0, neg = 0;
        for (int k = 1; ; ++k) {
            size_t g1 = (size_t)k * (3 * k - 1) / 2;
            if (g1 > n) break;
            seq_t term = new_p[n - g1];
            (k & 1) ? (pos += term) : (neg += term);

            size_t g2 = (size_t)k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            term = new_p[n - g2];
            (k & 1) ? (pos += term) : (neg += term);
        }
        new_p[n] = pos - neg;          /* positive minus negative */
    }

    p = new_p;
    p_cap = new_cap;
}

/* ---------- a(n) cache ---------- */
static seq_t *a_cache = NULL;      /* a_cache[1..cap-1] ; index 0 unused */
static size_t a_cap = 0;           /* number of elements stored */

/* Ensure that a_cache has room for indices up to limit. */
static void ensure_a_cache(idx_t limit) {
    if (limit < (idx_t)a_cap) return;                /* already sufficient */

    size_t old_cap = a_cap;
    size_t new_cap = (size_t)limit + 1;              /* indices 0..limit */

    seq_t *new_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_a) return;

    /* copy existing data */
    if (a_cache) {
        for (size_t i = 0; i < old_cap; ++i)
            new_a[i] = a_cache[i];
        free(a_cache);
    }

    /* zero‑initialize new slots */
    for (size_t i = old_cap; i < new_cap; ++i)
        new_a[i] = 0;

    a_cache = new_a;
    a_cap   = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined below offset */

    /* Build caches as needed */
    ensure_a_cache(n);
    ensure_partitions(n);

    /* Return cached value if available */
    if (a_cache[(size_t)n] != 0)
        return a_cache[(size_t)n];

    seq_t sum = 0;

    /* Enumerate divisors up to sqrt(n) without using sqrt() */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* term d * p(d) */
            sum += (seq_t)d * p[(size_t)d];

            idx_t q = n / d;
            if (q != d)                    /* add symmetric divisor */
                sum += (seq_t)q * p[(size_t)q];
        }
    }

    a_cache[(size_t)n] = sum;
    return sum;
}
