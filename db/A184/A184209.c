/*
 * A184209  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock sum
 *          greater than 6.
 * https://oeis.org/A184209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n walks on a 16‑state directed graph *
 *            whose adjacency matrix M[j][k] = 1 iff a+b+c+d > 6 for states *
 *            (a,b) = j, (c,d) = k. The starting state is arbitrary, * so a(n)
 *            is the sum of all entries of M^n. * * The function memoises
 *            results up to the largest n requested * and u...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static unsigned char M[16][16];
static bool adj_init = false;

static seq_t *a_cache      = NULL;   /* array of a(k) values */
static size_t a_cache_cap  = 0;      /* allocated capacity (>= n+1) */
static idx_t   a_cache_n   = 0;      /* highest n whose value is computed */

static seq_t curr[16];   /* path counts at current length (starting from any state) */
static seq_t next[16];   /* temporary array for the next step */

static void ensure_cache(idx_t n)
{
    /* initialise adjacency matrix once */
    if (!adj_init) {
        for (idx_t a = 0; a < 16; ++a)
            for (idx_t b = 0; b < 16; ++b)
                M[a][b] = 0;
        for (idx_t ai = 0; ai < 4; ++ai)
            for (idx_t bi = 0; bi < 4; ++bi) {
                idx_t a = ai * 4 + bi;
                for (idx_t ci = 0; ci < 4; ++ci)
                    for (idx_t di = 0; di < 4; ++di) {
                        idx_t b = ci * 4 + di;
                        if (ai + bi + ci + di > 6)
                            M[a][b] = 1;
                    }
            }
        adj_init = true;
    }

    if (n <= a_cache_n)
        return;                         /* already computed */

    /* grow cache if necessary */
    if (a_cache_cap < (size_t)(n + 1)) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : n + 1;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                    /* allocation failed */
        if (a_cache) {
            for (size_t i = 0; i <= (size_t)a_cache_n; ++i)
                tmp[i] = a_cache[i];
            free(a_cache);
        }
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* initialise DP if this is the first run */
    if (a_cache_n == 0) {
        for (idx_t k = 0; k < 16; ++k)
            curr[k] = (seq_t)1;          /* paths of length 0 end at each state */
        a_cache[0] = 16;                 /* unused but kept for completeness */
    }

    /* compute values from a_cache_n+1 up to n */
    for (idx_t t = a_cache_n + 1; t <= n; ++t) {
        for (idx_t k = 0; k < 16; ++k)
            next[k] = 0;

        for (idx_t j = 0; j < 16; ++j) {
            seq_t v = curr[j];
            if (!v)
                continue;
            for (idx_t k = 0; k < 16; ++k) {
                if (M[j][k])
                    next[k] += v;
            }
        }

        seq_t sum = 0;
        for (idx_t k = 0; k < 16; ++k)
            sum += next[k];

        a_cache[t] = sum;

        for (idx_t k = 0; k < 16; ++k)
            curr[k] = next[k];
    }

    a_cache_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, undefined for n==0 */
        return (seq_t)0;
    ensure_cache(n);
    return a_cache[n];
}
