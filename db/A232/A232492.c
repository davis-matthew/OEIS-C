/*
 * A232492  Number of symmetry classes of 3-eared triangulations of an n-gon.
 * https://oeis.org/A232492
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence -a(n) + 6*a(n-1) -10*a(n-2)
 *            -2*a(n-3) +12*a(n-4) +4*a(n-5) +8*a(n-6) -48*a(n-7) +32*a(n-8) =
 *            p(n), where p(n) = 0 for n < 6, p(6) = -1, p(7) = 5, p(8) = -9,
 *            p(9) = 4, p(10) = 2, p(11) = 6, p(12) = -8. Initial terms
 *            a(0)=a(1)=a(2)=a(3)=a(4)=0, a(5)=0. The seque...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of a[0..max] */
static size_t cache_cap = 0;         /* allocated size of cache */
static idx_t last_computed = (idx_t)-1; /* highest index computed, -1 if none */

/* coefficients of p(n) for 0 ≤ n ≤ 12 */
static const int p_coefs[13] =
{
    0, 0, 0, 0, 0, 0,          /* 0‑5 */
   -1, 5, -9, 4, 2, 6, -8      /* 6‑12 */
};

static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    size_t new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* For indices below the offset any value is acceptable */
    if (n < (idx_t)5) return (seq_t)0;

    ensure_capacity(n);

    /* Compute missing values from last_computed+1 up to n */
    idx_t start;
    if (last_computed == (idx_t)-1)       /* first call */
        start = 0;
    else
        start = last_computed + (idx_t)1;
    if (last_computed == (idx_t)-1) start = 0;

    for (idx_t i = start; i <= n; ++i)
    {
        /* Build right‑hand side of the recurrence */
        seq_t val = 0;
        if (i >= 1) val += 6 * cache[i - 1];
        if (i >= 2) val -= 10 * cache[i - 2];
        if (i >= 3) val -= 2 * cache[i - 3];
        if (i >= 4) val += 12 * cache[i - 4];
        if (i >= 5) val += 4 * cache[i - 5];
        if (i >= 6) val += 8 * cache[i - 6];
        if (i >= 7) val -= 48 * cache[i - 7];
        if (i >= 8) val += 32 * cache[i - 8];

        /* Subtract the inhomogeneous term p(i) */
        int p_i = 0;
        if (i <= 12) p_i = p_coefs[(size_t)i];
        if (p_i >= 0)
            val -= (seq_t)p_i;
        else
            val += (seq_t)(-p_i);

        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
