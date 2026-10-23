/*
 * A194765  Number of k such that {-k*2^(1/3)} > {-n*2^(1/3)}, where { } =
 *          fractional part.
 * https://oeis.org/A194765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k < n such that {k*α} < {n*α}, * where α =
 *            2^(1/3) and {x} denotes the fractional part of x
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   fenw_inc, fenw_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include "../utils/fenw_inc.h"
#include "../utils/fenw_sum.h"

/* Comparator for sorting indices by fractional part value */
static long double *g_s;  /* pointer to fractional part array used by cmp */
static int cmp_idx(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    long double va = g_s[ia];
    long double vb = g_s[ib];
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache of results */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If cache is too small, recompute up to n */
    if (n > cache_size)
    {
        idx_t N = n;                 /* rebuild up to N */
        long double alpha = cbrt((long double)2);  /* α = 2^(1/3) */

        /* allocate temporary arrays */
        long double *s = (long double *)malloc((N + 1) * sizeof(long double));
        int *order = (int *)malloc((N + 1) * sizeof(int));
        int *rank = (int *)malloc((N + 1) * sizeof(int));
        seq_t *new_cache = (seq_t *)malloc((N + 1) * sizeof(seq_t));

        /* compute fractional parts of k*α for k = 1..N */
        s[0] = 0.0L;                 /* unused */
        s[1] = fmodl(alpha, 1.0L);   /* fractional part of α */
        for (idx_t k = 2; k <= N; ++k)
        {
            s[k] = fmodl(s[k - 1] + alpha, 1.0L);
        }

        /* Fill order array */
        for (idx_t k = 1; k <= N; ++k)
            order[k] = (int)k;

        /* Sort indices by their fractional part */
        g_s = s;                     /* expose to comparator */
        qsort(order + 1, N, sizeof(int), cmp_idx);

        /* Build rank array */
        for (int i = 0; i < (int)N; ++i)
            rank[order[i + 1]] = i;  /* 0‑based rank */

        /* Fenwick tree for order statistics */
        seq_t *fenwick = (seq_t *)calloc(N + 1, sizeof(seq_t));
        for (idx_t k = 1; k <= N; ++k)
        {
            int r = rank[k];               /* 0‑based rank */
            seq_t smaller = fenw_sum(fenwick, (size_t)r);
            new_cache[k] = smaller;
            fenw_inc(fenwick, (size_t)N, (size_t)(r + 1));
        }

        /* free temporaries */
        free(s);
        free(order);
        free(rank);
        free(fenwick);

        /* replace old cache */
        free(cache);
        cache = new_cache;
        cache_size = N;
    }

    return cache[n];
}
