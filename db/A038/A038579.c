/*
 * A038579  Number of self-avoiding closed walks from 0 of area n in strip Z X
 *          {0,1,2}.
 * https://oeis.org/A038579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   (derived from the G.f.) Let f(n) be the sequence with
 *            1/(1‑x‑x²‑x³)=∑f(n)xⁿ (f(0)=1,f(1)=1,…). Let g(n)=∑f(k)f(n‑k)
 *            (the square of the above series). g satisfies the linear
 *            recurrence g(n)=2g(n‑1)+g(n‑2)−3g(n‑4)−2g(n‑5)−g(n‑6) (n≥6) with
 *            initial values g(0)=1,g(1)=2,g(2)=5,g(3)=12,g(4)=26,g(5)=56...
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Pre‑computed initial values of g(n) for 0≤n≤5. */
static const seq_t g_init[6] = { 1, 2, 5, 12, 26, 56 };

/* Cached array of g(n) values. */
static seq_t *g_arr      = NULL;   /* array pointer */
static idx_t  g_max      = (idx_t)-1;   /* highest computed index */
static size_t g_cap      = 0;    /* allocated size */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough. */
    if (g_arr == NULL) {
        g_cap = (size_t)(n + 1);
        if (g_cap < 6) g_cap = 6;
        g_arr = (seq_t*)malloc(g_cap * sizeof(seq_t));
        if (!g_arr) return (seq_t)0;   /* allocation failure */
        /* initialise the first six values */
        for (idx_t i = 0; i < 6; ++i)
            g_arr[i] = g_init[i];
        g_max = 5;
    } else if ((size_t)n >= g_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 6) new_cap = 6;
        seq_t *tmp = (seq_t*)realloc(g_arr, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        /* initialise any new indices up to 5 if needed */
        if (g_cap < 6) {
            for (idx_t i = g_cap; i < 6; ++i)
                tmp[i] = g_init[i];
        }
        g_arr = tmp;
        g_cap = new_cap;
        if (g_max < 5) g_max = 5;
    }

    /* Compute any missing g(n) values up to n. */
    if (n > g_max) {
        idx_t start = (g_max < 5 ? 6 : g_max + 1);
        for (idx_t i = start; i <= n; ++i) {
            seq_t val =  (seq_t)2 * g_arr[i-1]          /* 2g(n-1)  */
                       + g_arr[i-2];                      /* +g(n-2)  */
            val -= (seq_t)3 * g_arr[i-4];             /* -3g(n-4) */
            val -= (seq_t)2 * g_arr[i-5];             /* -2g(n-5) */
            val -=          g_arr[i-6];              /* -g(n-6)  */
            g_arr[i] = val;
        }
        g_max = n;
    }

    /* Compute the final term a(n). */
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)2 * g_arr[n];               /* 2·g(n)  */
    if (n >= 3) res += (seq_t)4 * g_arr[n-3];       /* +4·g(n‑3) */
    if (n >= 4) res += (seq_t)4 * g_arr[n-4];       /* +4·g(n‑4) */
    return res;
}
