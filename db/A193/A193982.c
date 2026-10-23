/*
 * A193982  Number of ways to arrange 4 nonattacking triangular rooks on an
 *          nXnXn triangular grid.
 * https://oeis.org/A193982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) +2*a(n-3) +27*a(n-4) -36*a(n-5) *
 *            +36*a(n-7) -27*a(n-8) -2*a(n-9) +12*a(n-10) -6*a(n-11) +a(n-12)
 *            * Initial values: * a(1)=a(2)=a(3)=a(4)=a(5)=0, * a(6)=18,
 *            a(7)=233, a(8)=1449, a(9)=6213, a(10)=20993, * a(11)=59943,
 *            a(12)=150903
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 36, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc, realloc */

static seq_t *cache = NULL;      /* 1‑indexed cache */
static idx_t cache_len = 0;      /* largest n already stored */

/* Return a(n) for the OEIS sequence A193982. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* out of offset – harmless */

    /* Grow the cache to at least size n+1 (index 0 unused) */
    if (n > cache_len) {
        size_t new_len = (size_t)n + 1;          /* indices 0 … n */
        size_t old_len = (size_t)cache_len + 1;
        seq_t *tmp;

        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (tmp == NULL)
            return (seq_t)0;    /* allocation failure – abort gracefully */
        cache = tmp;

        /* Initial values for indices 1 … 12. */
        static const seq_t init[13] = {
            0,      /* 0‑index (unused) */
            0, 0, 0, 0, 0,  /* a(1)…a(5) */
            18,      /* a(6) */
            233,     /* a(7) */
            1449,    /* a(8) */
            6213,    /* a(9) */
            20993,   /* a(10) */
            59943,   /* a(11) */
            150903   /* a(12) */
        };

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                /* Apply the linear recurrence. All operations are on seq_t. */
                seq_t val = cache[i - 1] * (seq_t)6;
                val -= cache[i - 2] * (seq_t)12;
                val += cache[i - 3] * (seq_t)2;
                val += cache[i - 4] * (seq_t)27;
                val -= cache[i - 5] * (seq_t)36;
                val += cache[i - 7] * (seq_t)36;
                val -= cache[i - 8] * (seq_t)27;
                val -= cache[i - 9] * (seq_t)2;
                val += cache[i -10] * (seq_t)12;
                val -= cache[i -11] * (seq_t)6;
                val += cache[i -12];
                cache[i] = val;
            }
        }
        cache_len = n;
    }

    return cache[n];
}
