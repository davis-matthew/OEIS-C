/*
 * A008128  Coordination sequence T2 for Zeolite Code LEV.
 * https://oeis.org/A008128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A008128 (Coordination sequence T2 for Zeolite Code LEV):
   a(n) = N[n] - Σ_{k=1}^{19} D[k] * a(n-k),  D[0]=1
   D = { 1, -1, 1, 0, 0, -2, 2, -3, 1, -1, 1, -1, 3, -2, 2, 0, 0, -1, 1, -1 }
   N = { 1, 3, 7, 14, 22, 32, 44, 57, 67, 71, 71, 67, 57, 44, 32, 22, 14, 7, 3, 1 }
   For n ≥ 20, N[n] = 0. */

#include <stdlib.h>

static const int D[20] = {
    1,  /* D0 */
   -1,  /* D1 */
    1,  /* D2 */
    0,  /* D3 */
    0,  /* D4 */
   -2,  /* D5 */
    2,  /* D6 */
   -3,  /* D7 */
    1,  /* D8 */
   -1,  /* D9 */
    1,  /* D10 */
   -1,  /* D11 */
    3,  /* D12 */
   -2,  /* D13 */
    2,  /* D14 */
    0,  /* D15 */
    0,  /* D16 */
   -1,  /* D17 */
    1,  /* D18 */
   -1   /* D19 */
};

static const int N[20] = {
    1,  /* N0 */
    3,  /* N1 */
    7,  /* N2 */
   14,  /* N3 */
   22,  /* N4 */
   32,  /* N5 */
   44,  /* N6 */
   57,  /* N7 */
   67,  /* N8 */
   71,  /* N9 */
   71,  /* N10 */
   67,  /* N11 */
   57,  /* N12 */
   44,  /* N13 */
   32,  /* N14 */
   22,  /* N15 */
   14,  /* N16 */
    7,  /* N17 */
    3,  /* N18 */
    1   /* N19 */
};

static seq_t *cache = NULL;   /* memoized values */
static size_t cache_size = 0; /* number of computed entries */

/* Ensure cache has values for indices 0 .. n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;

    size_t new_size = (size_t)n + 1;

    /* Allocate or reallocate.  Use explicit cast for C++. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (cache == NULL) abort();
    } else if (new_size > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* Compute missing entries.  cache_size is the number of already
       computed values (indices 0 .. cache_size-1). */
    for (size_t i = cache_size; i < new_size; ++i) {
        seq_t val = (i < 20) ? (seq_t)N[i] : (seq_t)0;
        seq_t sum_pos = 0; /* sum of coeff>0 * a(n-k) */
        seq_t sum_neg = 0; /* sum of (-coeff) * a(n-k) for coeff<0 */

        for (int j = 1; j <= 19; ++j) {
            if (i < (size_t)j) break;     /* a[n-k] = 0 for k > n */
            int coeff = D[j];
            if (coeff > 0) {
                sum_pos += (seq_t)coeff * cache[i - j];
            } else if (coeff < 0) {
                sum_neg += (seq_t)(-coeff) * cache[i - j];
            }
            /* coeff == 0: ignore */
        }

        val = val + sum_neg;
        val = val - sum_pos;
        cache[i] = val;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
