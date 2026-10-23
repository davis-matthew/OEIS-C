/*
 * A170410  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170410
    Number of reduced words of length n in Coxeter group
    on 17 generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
    The generating function is

          N(t) = t^44 + 2*t^43 + … + 2*t + 1
          D(t) = 1 – 15*t – 15*t^2 – … – 15*t^43 + 120*t^44

    The coefficients a(n) satisfy the linear recurrence

          a(n) = 15·∑_{k=1}^{min(n,43)} a(n-k) – 120·a(n-44) + N_n
          N_0 = 1,  N_1 … N_43 = 2,  N_44 = 1,  otherwise 0

    The implementation below computes the terms iteratively, caches all
    previous values, and works for any integer type seq_t.  The
    computation is O(n·43) time and O(n) memory.  */
    
#include <stdlib.h>
#include <stddef.h>

static seq_t *coef_cache = NULL;   /* cached coefficients */
static idx_t  cache_size = 0;      /* allocated size            */
static idx_t  cache_last  = 0;     /* highest index computed    */

/* Grow the cache to hold indices up to n (inclusive).  If a reallocation
   occurs the initial value a(0)=1 is (re)set. */
static void
grow_cache(idx_t n)
{
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size < 16 ? 16 : cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(coef_cache, (size_t)new_size * sizeof(seq_t));
        coef_cache = tmp;
        cache_size = new_size;
        /* initialise the first term on allocation */
        coef_cache[0] = (seq_t)1;
        cache_last   = 0;
    }
}

/* ------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* initialise the cache on first use */
    if (cache_size == 0) {
        grow_cache(n);
    } else {
        grow_cache(n);
    }

    /* return the value if already computed */
    if (n <= cache_last) {
        return coef_cache[(size_t)n];
    }

    /* compute new terms from cache_last+1 up to n inclusive */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* right‑hand side of the recurrence */
        seq_t Ni;
        if (i == 0 || i == 44) {
            Ni = (seq_t)1;
        } else if (i <= 43) {
            Ni = (seq_t)2;
        } else {
            Ni = (seq_t)0;
        }

        /* sum of the previous up to 43 terms */
        seq_t sum = (seq_t)0;
        idx_t lim = (i <= 43) ? i : (idx_t)43;
        for (idx_t k = 1; k <= lim; ++k) {
            sum = sum + coef_cache[(size_t)(i - k)];
        }

        /* a(i) = Ni + 15*sum – 120*a(i-44)  (if i>=44) */
        seq_t term = Ni + (seq_t)15 * sum;
        if (i >= 44) {
            term = term - (seq_t)120 * coef_cache[(size_t)(i - 44)];
        }

        coef_cache[(size_t)i] = term;
    }

    cache_last = n;
    return coef_cache[(size_t)n];
}
