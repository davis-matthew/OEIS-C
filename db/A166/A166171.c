/*
 * A166171  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = Ncoeff[n] + 37*(a(n-1)+a(n-2)+...+a(n-9)) -
 *            703*a(n-10) where Ncoeff[0..10] = {1,2,2,2,2,2,2,2,2,2,1} and
 *            Ncoeff[n] = 0 for n>10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global cache of computed values */
static seq_t *cache = NULL;           /* array of a(i) */
static idx_t  cache_size = 0;         /* number of allocated elements */
static idx_t  last_computed = (idx_t)-1;   /* highest index already computed */

static const int Ncoeff[11] = {1,2,2,2,2,2,2,2,2,2,1};

/* Ensure values up to idx are available in the cache. */
static void ensure_up_to(idx_t idx)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc((idx + 1) * sizeof(seq_t));
        cache_size = idx + 1;
    } else if (cache_size <= idx) {
        cache = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        cache_size = idx + 1;
    }

    /* Compute from the next missing index up to idx. */
    if (last_computed == (idx_t)-1) {          /* first call; compute a(0) */
        cache[0] = (seq_t)Ncoeff[0];
        last_computed = 0;
    }

    if (last_computed >= idx) return;          /* already computed */

    for (idx_t i = last_computed + 1; i <= idx; ++i) {
        seq_t sum9 = 0;
        for (idx_t k = 1; k <= 9; ++k) {
            if (i >= k) sum9 += cache[i - k];
        }

        seq_t const_term = (i <= 10) ? (seq_t)Ncoeff[i] : (seq_t)0;
        seq_t part       = (seq_t)37 * sum9;      /* 37 * sum of last 9 terms */
        seq_t sub        = (i >= 10) ? (seq_t)703 * cache[i - 10] : (seq_t)0;

        cache[i] = part + const_term - sub;     /* recurrence */
    }

    last_computed = idx;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
