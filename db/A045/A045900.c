/*
 * A045900  Hypothetical numbers of terms in general expressions for
 *          coefficients of Lovelock Lagrangians.
 * https://oeis.org/A045900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  
   a(0) = 1, a(1) = 1, a(2) = 3,  
   for n ≥ 3  
   a(n) = 3*a(n-1) – 3*a(n-2) + a(n-3) + p(n) – 3*p(n-1) + 3*p(n-2) – p(n-3) + 2^(2*n-5)  
   where p(n) is the partition function with p(0)=1. */
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

/* Static caches for sequence values and partition numbers */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;
static idx_t a_used = 0;      /* next uncomputed index */

static seq_t *p_cache = NULL;
static size_t p_cap = 0;
static idx_t p_used = 0;      /* next uncomputed index */

/* Ensure the array is large enough for indices up to n */
static void ensure_array(size_t *cap, seq_t **array, idx_t n)
{
    if ((size_t)n + 1 > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : *cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        *array = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        /* new entries will be filled before use */
        *cap = new_cap;
    }
}

/* Compute partition numbers up to n, using pentagonal recurrence */
static void compute_partitions(idx_t n)
{
    if (p_used > n) return;

    ensure_array(&p_cap, &p_cache, n);

    /* base value */
    if (p_used == 0) {
        p_cache[0] = 1;
        p_used = 1;
    }
    for (idx_t i = p_used; i <= n; ++i) {
        if (i == 0) continue;      /* already set */
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t j = 1; ; ++j) {
            idx_t pent1 = j * (3 * j - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p_cache[i - pent1];
            if (j & 1U) pos += term; else neg += term;
            idx_t pent2 = j * (3 * j + 1) / 2;
            if (pent2 > i) continue;
            term = p_cache[i - pent2];
            if (j & 1U) pos += term; else neg += term;
        }
        p_cache[i] = pos - neg;   /* pos >= neg guaranteed */
    }
    p_used = n + 1;
}

/* Compute sequence a up to n using the recurrence */
static void compute_a(idx_t n)
{
    if (a_used > n) return;

    ensure_array(&a_cap, &a_cache, n);

    /* initialise base values */
    if (a_used == 0) {
        a_cache[0] = 1;
        a_cache[1] = 1;
        a_cache[2] = 3;
        a_used = 3;
    } else if (a_used == 1) {
        a_cache[1] = 1;
        a_used = 2;
    } else if (a_used == 2) {
        a_cache[2] = 3;
        a_used = 3;
    }

    /* ensure partitions up to required n are available */
    compute_partitions(n);

    for (idx_t i = a_used; i <= n; ++i) {
        if (i <= 2) continue;   /* base cases already set */

        seq_t powterm = pow2((idx_t)(2 * i - 5));

        seq_t pos = mul_small(a_cache[i - 1], 3);   /* 3*a[i-1] */
        pos += a_cache[i - 3];
        pos += p_cache[i];
        pos += mul_small(p_cache[i - 2], 3);        /* 3*p[i-2] */
        pos += powterm;

        seq_t neg = mul_small(a_cache[i - 2], 3);   /* 3*a[i-2] */
        neg += mul_small(p_cache[i - 1], 3);        /* 3*p[i-1] */
        neg += p_cache[i - 3];

        a_cache[i] = pos - neg;
    }
    a_used = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 3) {
        /* ensure base values are ready */
        compute_a(2);
        return a_cache[n];
    }
    compute_a(n);
    return a_cache[n];
}
