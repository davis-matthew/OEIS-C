/*
 * A211459  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 21*a(n-3) + 16*a(n-4) + 29*a(n-5) -
 *            34*a(n-6) - 6*a(n-7) + 12*a(n-8) for n>8, a(1)=20, a(2)=44,
 *            a(3)=92, a(4)=178, a(5)=348, a(6)=658, a(7)=1260, a(8)=2382
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define CACHE_CAP 20000            /* enough for n up to about 10 000 */

static seq_t cache[CACHE_CAP + 1];
static idx_t computed = 0;          /* highest index already computed */

/* Initial values for n = 1 … 8 */
static const seq_t base_vals[8] = {20, 44, 92, 178,
                                  348, 658, 1260, 2382};

seq_t a(idx_t n)
{
    /* Guard against calling with n==0 – return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Initialise base values on the first call */
    if (computed == 0)
    {
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = base_vals[i - 1];
        computed = 8;
    }

    /* If the requested index is already known, return it */
    if (n <= computed)
        return cache[n];

    /* If n exceeds the static cache, fall back to a dynamically
       allocated buffer that works for both integral and arbitrary‑
       precision sequence types. */
    if (n > CACHE_CAP)
    {
        size_t cap = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *dyn = new seq_t[cap];
#else
        seq_t *dyn = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!dyn) return (seq_t)0;              /* out of memory – unlikely */
#endif
        for (idx_t i = 1; i <= 8; ++i)
            dyn[i] = base_vals[i - 1];

        for (idx_t i = computed + 1; i <= n; ++i)
        {
            dyn[i] = ((seq_t)4) * dyn[i - 1] + dyn[i - 2]
                    - ((seq_t)21) * dyn[i - 3]
                    + ((seq_t)16) * dyn[i - 4]
                    + ((seq_t)29) * dyn[i - 5]
                    - ((seq_t)34) * dyn[i - 6]
                    - ((seq_t)6)  * dyn[i - 7]
                    + ((seq_t)12) * dyn[i - 8];
        }

        seq_t result = dyn[n];
#ifdef __cplusplus
        delete[] dyn;
#else
        free(dyn);
#endif
        return result;
    }

    /* Compute all missing terms up to n using the static cache */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        cache[i] = ((seq_t)4) * cache[i - 1] + cache[i - 2]
                 - ((seq_t)21) * cache[i - 3]
                 + ((seq_t)16) * cache[i - 4]
                 + ((seq_t)29) * cache[i - 5]
                 - ((seq_t)34) * cache[i - 6]
                 - ((seq_t)6)  * cache[i - 7]
                 + ((seq_t)12) * cache[i - 8];
    }
    computed = n;
    return cache[n];
}
