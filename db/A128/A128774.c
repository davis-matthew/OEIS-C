/*
 * A128774  a(n) = numerator of r(n): r(1)=1, r(n+1) =
 *          [b(1,n);b(2,n),...,b(n,n)], a continued fraction of rational
 *          terms, where {b(k,n)} is the permutation of the first n terms of
 *          {r(k)} such that r(n+1) is minimized.
 * https://oeis.org/A128774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of r(n) where * r(1) = 1 * r(n+1) = continued
 *            fraction [b1,b2,…,bn] with bi a permutation of * {r(1),…,r(n)}
 *            that minimises r(n+1). * The optimal permutation is obtained by
 *            starting with the smallest * element, then alternately taking
 *            the largest remaining and the * smallest rema...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_gcd.h"

struct Fraction {
    seq_t num;
    seq_t den;
};

/* compare fractions: a<b -> -1, a==b -> 0, a>b -> 1 */
static int cmp_frac(const struct Fraction *a, const struct Fraction *b)
{
    seq_t left  = a->num * b->den;
    seq_t right = b->num * a->den;
    if (left  < right) return -1;
    if (left  > right) return  1;
    return 0;
}

/* reduce a fraction by dividing by gcd of numerator and denominator */
static struct Fraction reduce_frac(struct Fraction f)
{
    seq_t g = seq_gcd(f.num, f.den);
    if (g != 0) {
        f.num /= g;
        f.den /= g;
    }
    return f;
}

/* add two fractions: a + b */
static struct Fraction add_frac(const struct Fraction *a,
                                const struct Fraction *b)
{
    struct Fraction r;
    r.num = a->num * b->den + a->den * b->num;
    r.den = a->den * b->den;
    return reduce_frac(r);
}

/* reciprocal of a fraction */
static struct Fraction recip_frac(const struct Fraction *f)
{
    struct Fraction r = { f->den, f->num };
    return r;
}

/* global memoisation data */
static struct Fraction *fracs   = NULL;   /* 1‑indexed array of r(k) */
static size_t          fracs_cap = 0;
static idx_t           max_cached = 0;     /* largest n already computed */

static idx_t *sorted_idx = NULL;          /* indices of r(1)…r(k) sorted by value */
static size_t sorted_cap = 0;
static size_t sorted_len = 0;

/* ensure fracs array has capacity at least n+1 (index 0 unused) */
static void ensure_cap(size_t n)
{
    if (fracs_cap < n + 1) {
        size_t new_cap = (fracs_cap == 0) ? 16 : fracs_cap * 2;
        while (new_cap < n + 1) new_cap <<= 1;
        struct Fraction *tmp = (struct Fraction *)realloc(fracs, new_cap * sizeof(struct Fraction));
        if (!tmp) abort();
        fracs = tmp;
        fracs_cap = new_cap;
    }
}

/* ensure sorted_idx has capacity at least n */
static void ensure_sorted_cap(size_t n)
{
    if (sorted_cap < n) {
        size_t new_cap = (sorted_cap == 0) ? 16 : sorted_cap * 2;
        while (new_cap < n) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(sorted_idx, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        sorted_idx = tmp;
        sorted_cap = new_cap;
    }
}

/* insert index idx into sorted_idx maintaining ascending order of r[idx] */
static void insert_sorted(idx_t idx)
{
    ensure_sorted_cap(sorted_len + 1);
    size_t lo = 0, hi = sorted_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cmp_frac(&fracs[sorted_idx[mid]], &fracs[idx]) <= 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* shift elements to make space */
    for (size_t i = sorted_len; i > lo; --i)
        sorted_idx[i] = sorted_idx[i-1];
    sorted_idx[lo] = idx;
    ++sorted_len;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 safely */

    if (max_cached < n) {
        /* extend storage to cover n */
        ensure_cap((size_t)n);
        ensure_sorted_cap((size_t)n);

        /* base case: r(1) = 1/1 */
        if (max_cached == 0) {
            fracs[1].num = 1;
            fracs[1].den = 1;
            insert_sorted(1);
            max_cached = 1;
        }

        /* compute successive r(k) for k = max_cached+1 … n */
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            size_t m = (size_t)(k - 1);          /* number of previous terms */

            /* permutation of indices: smallest first, then largest,
             * then smallest remaining, etc. */
            idx_t *perm = (idx_t *)malloc(m * sizeof(idx_t));
            if (!perm) abort();

            perm[0] = sorted_idx[0];
            size_t l = 1, r = m - 1;
            size_t pos = 1;
            while (l <= r) {
                perm[pos++] = sorted_idx[r--];
                if (l <= r)
                    perm[pos++] = sorted_idx[l++];
            }

            /* evaluate continued fraction from back to front */
            struct Fraction val = fracs[perm[m - 1]];
            for (size_t i = m - 1; i > 0; --i) {
                struct Fraction rec = recip_frac(&val);
                val = add_frac(&fracs[perm[i - 1]], &rec);
            }

            free(perm);

            fracs[k] = val;
            insert_sorted(k);
        }
        max_cached = n;
    }

    return fracs[n].num;
}
