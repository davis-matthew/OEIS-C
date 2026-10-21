/*
 * A216499  The maximum possible number of rooted triples consistent with any
 *          galled-tree (level-1 phylogenetic network) containing exactly n
 *          leaves.
 * https://oeis.org/A216499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = max_{1<=i<=n}[
 *            C(i,3)+2*C(i,2)*(n-i)+i*C(n-i,2)+a(n-i) ] (n>0)
 *            C(x,2)=x*(x-1)/2, C(x,3)=x*(x-1)*(x-2)/6 The function is
 *            memoised; subsequent calls are O(1)
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* binomial coefficients using 64‑bit intermediate arithmetic */
static inline seq_t binom2(idx_t x)
{
    if (x < 2) return 0;
    return (seq_t)(x * (x - 1) / 2);
}

static inline seq_t binom3(idx_t x)
{
    if (x < 3) return 0;
    return (seq_t)(x * (x - 1) * (x - 2) / 6);
}

/* dynamic cache for a(n) */
static seq_t *a_cache = NULL;   /* array of a[n] values          */
static size_t  a_cap   = 0;      /* current allocated size        */
static idx_t   a_last  = 0;      /* highest index computed so far */

/* make sure the cache can hold indices up to n */
static void a_ensure(idx_t n)
{
    if (n < a_cap) return;
    size_t new_cap = n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!a_cache) abort();               /* out of memory */
    /* initialise newly allocated elements to 0 */
    if (new_cap > a_cap)
        memset(a_cache + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
    a_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* handle n==0 immediately */
    if (n == 0) return 0;

    if (a_cache == NULL)
        a_ensure(0);                     /* allocate slot 0 */
    /* initialise a(0) if it hasn't been set yet */
    if (a_cap == 0 || a_cap <= 1)
        a_cache[0] = 0;
    a_last = 0;                           /* no values computed yet */

    /* if we already have the value, return it */
    if (n <= a_last)
        return a_cache[n];

    /* compute missing values up to n */
    a_ensure(n);
    for (idx_t k = a_last + 1; k <= n; ++k)
    {
        seq_t best = 0;
        for (idx_t i = 1; i <= k; ++i)
        {
            seq_t term = binom3(i);
            term += (seq_t)2 * binom2(i) * (seq_t)(k - i);
            term += i * binom2(k - i);
            term += a_cache[k - i];
            if (term > best) best = term;
        }
        a_cache[k] = best;
    }
    a_last = n;
    return a_cache[n];
}
