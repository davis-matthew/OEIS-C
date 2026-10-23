/*
 * A168787  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*Σ_{i=1}^{18}a(n-i) - 78*a(n-19) + p_n, where p_n = 1
 *            for n=0 or 19, p_n = 2 for 1≤n≤18, and p_n = 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cached values of the sequence   */
static size_t cap   = 0;          /* allocated capacity of cache      */
static size_t sz    = 0;          /* number of values stored in cache */

/* Helper: grow cache so that it contains a(0)..a(n)   */
static void grow_cache(idx_t n)
{
    if (sz > n) return;

    /* ensure enough space */
    size_t need = (size_t)n + 1;
    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* sum of the last 18 values needed for the recurrence */
    seq_t sum18 = 0;
    if (sz) {
        idx_t start = (sz >= 18 ? sz - 18 : 0);
        for (idx_t k = start; k < sz; ++k) sum18 += cache[(size_t)k];
    }

    for (idx_t i = sz; i <= n; ++i) {
        /* compute p_n */
        seq_t p = 0;
        if (i == 0) {
            p = 1;
        } else if (i <= 18) {
            p = 2;
        } else if (i == 19) {
            p = 1;
        }

        /* a(i) = 12*sum18 - 78*a(i-19) + p */
        seq_t val = sum18 * (seq_t)12;
        if (i >= 19)  val -= cache[(size_t)(i - 19)] * (seq_t)78;
        val += p;
        cache[(size_t)i] = val;

        /* update sum18 for next iteration */
        sum18 += val;
        if (i >= 18) sum18 -= cache[(size_t)(i - 18)];
    }

    sz = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[(size_t)n];
}
