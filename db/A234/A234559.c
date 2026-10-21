/*
 * A234559  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n‑1)+30*a(n‑2)-132*a(n‑3)-331*a(n‑4)+1720*a(n‑5)+1530*a(n‑6)
 *              -11280*a(n‑7)-1324*a(n‑8)+39136*a(n‑9)-12360*a(n‑10)-67968*a(n‑11)
 *              +39456*a(n‑12)+46080*a(n‑13)-34560*a(n‑14)  for n ≥ 15
 * Initial values: a(1)=618, a(2)=1240, a(3)=2390, a(4)=5710, a(5)=12750,
 *                 a(6)=33946, a(7)=83438, a(8)=238306, a(9)=621918,
 *                 a(10)=1862530, a(11)=5040110, a(12)=15596050,
 *                 a(13)=43132350, a(14)=136586626 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* 1‑based index: cache[i] stores a(i) */
static size_t cap = 0;               /* current allocated capacity (indices 0..cap‑1) */
static idx_t last_computed = 0;      /* largest n for which a(n) is known (>=14 after init) */
static bool init_done = false;       /* whether initial terms have been filled */

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n + 1 <= cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* abort on allocation failure */
    /* Zero new elements so that a(n)=0 for n < current cap is detected. */
    for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cap = new_cap;
}

/* Initial 14 terms of the sequence (index 1…14). */
static const seq_t init[14] = {
    618ULL,
    1240ULL,
    2390ULL,
    5710ULL,
    12750ULL,
    33946ULL,
    83438ULL,
    238306ULL,
    621918ULL,
    1862530ULL,
    5040110ULL,
    15596050ULL,
    43132350ULL,
    136586626ULL
};

/* Coefficients for the recurrence used to compute a(n) for n ≥ 15. */
static const int  pos_idx[7]  = {1, 2, 5, 6, 9,12,13};
static const seq_t pos_val[7] = {4ULL,30ULL,1720ULL,1530ULL,39136ULL,39456ULL,46080ULL};
static const int  neg_idx[7]  = {3, 4, 7, 8,10,11,14};
static const seq_t neg_val[7] = {132ULL,331ULL,11280ULL,1324ULL,12360ULL,67968ULL,34560ULL};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n < 1 */
    if (n <= 14) return init[n - 1];          /* direct lookup */

    ensure_cache_cap(n);

    if (!init_done) {                         /* fill the 14 initial terms once */
        for (size_t i = 1; i <= 14; ++i) {
            cache[i] = init[i - 1];
        }
        init_done = true;
        last_computed = 14;
    }

    /* Compute missing values from last_computed+1 up to n. */
    idx_t start = (last_computed + 1 > 15) ? last_computed + 1 : 15;
    for (idx_t i = start; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 7; ++k) {
            pos += cache[i - pos_idx[k]] * pos_val[k];
            neg += cache[i - neg_idx[k]] * neg_val[k];
        }
        cache[i] = pos - neg;
    }
    if (n > last_computed) last_computed = n;

    return cache[n];
}
