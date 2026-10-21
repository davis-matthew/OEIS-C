/*
 * A098696  Main diagonal of array in A073146.
 * https://oeis.org/A098696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(n,k) * A000670(2*n-k) (A000670 = Fubini
 *            numbers)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* seq_t, idx_t are provided by the build system */

seq_t a(idx_t n)
{
    /* static caches for computed values */
    static seq_t *a_cache = NULL;   /* a_cache[i] holds a(i) */
    static seq_t *g_cache = NULL;   /* g_cache[j] holds A000670(j) */
    static idx_t a_max = 0;         /* largest n already computed in a_cache */
    static idx_t g_max = 0;         /* largest index computed in g_cache */

    /* first call: initialise caches */
    if (!a_cache) {
        a_cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!a_cache) return 0;     /* out‑of‑memory abort */
        a_cache[0] = 1;             /* a(0) = 1 */
        a_max = 0;
    }
    if (!g_cache) {
        g_cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!g_cache) return 0;
        g_cache[0] = 1;             /* G(0) = 1 (Fubini number) */
        g_max = 0;
    }

    /* return cached value immediately */
    if (n <= a_max) return a_cache[n];

    /* Expand a_cache up to index n */
    seq_t *new_a = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!new_a) return 0;
    a_cache = new_a;
    for (idx_t i = a_max + 1; i <= n; ++i) a_cache[i] = 0;

    /* For each new index cur, ensure g_cache up to 2*cur is ready */
    for (idx_t cur = a_max + 1; cur <= n; ++cur) {
        idx_t need_g = 2 * cur;
        if (g_max < need_g) {
            seq_t *new_g = (seq_t *)realloc(g_cache, (need_g + 1) * sizeof(seq_t));
            if (!new_g) return 0;
            g_cache = new_g;
            for (idx_t j = g_max + 1; j <= need_g; ++j) {
                /*   G(j) = Σ_{k=0}^{j-1} C(j,k) * G(k)   */
                seq_t sum = 0;
                seq_t comb = 1;           /* C(j,0) */
                for (idx_t k = 0; k < j; ++k) {
                    sum += comb * g_cache[k];
                    /* update comb to C(j,k+1) */
                    comb = comb * (seq_t)(j - k) / (seq_t)(k + 1);
                }
                g_cache[j] = sum;
            }
            g_max = need_g;
        }

        /*   a(cur) = Σ_{k=0}^{cur} C(cur,k) * G(2*cur - k)   */
        seq_t sum = 0;
        seq_t comb = 1;               /* C(cur,0) */
        for (idx_t k = 0; k <= cur; ++k) {
            sum += comb * g_cache[2 * cur - k];
            if (k < cur) {
                comb = comb * (seq_t)(cur - k) / (seq_t)(k + 1);
            }
        }
        a_cache[cur] = sum;
    }

    a_max = n;
    return a_cache[n];
}
