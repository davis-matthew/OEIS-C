/*
 * A177197  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=6, k=-1 and l=1.
 * https://oeis.org/A177197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177197
 * Recurrence:
 *   a(0) = 1
 *   a(1) = 6
 *   for n ≥ 1:
 *     a(n+1) = ∑_{p=0}^{n} ( a(p) * a(n-p) + k ) + l
 *   with k = -1, l = 1
 *   => a(n+1) = ∑_{p=0}^{n} a(p) * a(n-p) - n
 */

#include <stdlib.h>

/* static cache for memoised terms */
static seq_t *cache   = NULL;    /* array of computed terms          */
static size_t  cap    = 0;       /* current capacity of cache       */
static idx_t   len    = 0;       /* number of stored terms           */

/* Ensure cache contains terms up to and including index n. */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)len)   /* already present */
        return;

    /* Grow capacity if necessary */
    if (cap == 0)
        cap = 4;
    while (cap <= (size_t)n)
        cap <<= 1;                   /* double until sufficient */

    /* Reallocate memory for new capacity */
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                     /* allocation failed */
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = len; i <= n; ++i)
    {
        if (i == 0) { cache[0] = 1; continue; }
        if (i == 1) { cache[1] = 6; continue; }

        /* a(i) = Σ_{p=0}^{i-1} a(p)*a(i-1-p) - (i-1) */
        idx_t m   = i - 1;               /* convolution length   */
        seq_t sum = 0;

        /* Exploit symmetry: compute half and double */
        for (idx_t p = 0; p <= m / 2; ++p)
        {
            seq_t prod = cache[p] * cache[m - p];
            if (p < m - p)
                sum += prod + prod;   /* two off‑diagonal terms */
            else
                sum += prod;           /* middle term when m is even */
        }

        sum -= (seq_t)m;               /* subtract (i-1) */
        cache[i] = sum;
    }

    len = n + 1;   /* terms [0 .. n] are now stored */
}

/* Public function: sequence value at index n. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
