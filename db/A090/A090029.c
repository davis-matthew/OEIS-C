/*
 * A090029  Number of distinct lines through the origin in 7-dimensional cube
 *          of side length n.
 * https://oeis.org/A090029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^7 – 1 – Σ_{j=2}^{n+1} a(⌊n/j⌋) (initial value: a(0)
 *            = 0) The recurrence uses only previously computed values. A
 *            memoised array is kept so that repeated or out‑of‑order calls
 *            are handled efficiently
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static seq_t *cache = NULL;         /* cached a[0 .. cache_cap-1]      */
static size_t cache_cap = 0;         /* current array size (number of entries) */

/* public sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* enlarge cache if necessary */
    size_t start = cache_cap;
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        if (new_cap > cache_cap)                 /* initialise new elements */
            memset(tmp + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values in increasing order */
    for (size_t idx = start; idx <= (size_t)n; ++idx) {
        if (idx == 0) {                         /* a(0) = 0 */
            cache[0] = 0;
            continue;
        }

        /* total = (idx+1)^7 – 1 */
        seq_t base = (seq_t)idx + 1;
        seq_t power = 1;
        for (int k = 0; k < 7; ++k) power *= base;  /* multiply 7 times */
        seq_t total = power - 1;

        /* sum_{j=2}^{idx+1} a(floor(idx/j)) using grouping */
        seq_t sum = 0;
        idx_t j = 2;
        idx_t N1 = (idx_t)idx + 1;                 /* upper limit of j   */

        while (j <= N1) {
            idx_t v = (idx_t)idx / j;              /* floor(idx/j)       */
            idx_t last_j;
            if (v == 0)
                last_j = N1;                       /* j > idx → floor = 0   */
            else
                last_j = (idx_t)idx / v;           /* last j with this v   */

            if (last_j > N1) last_j = N1;          /* safety                */
            seq_t group_size = (seq_t)(last_j - j + 1);
            sum += cache[v] * group_size;
            j = last_j + 1;
        }

        cache[idx] = total - sum;                 /* recurrence          */
    }

    return cache[(size_t)n];
}
