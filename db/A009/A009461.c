/*
 * A009461  Expansion of e.g.f.: sin(log(1+x))/exp(x).
 * https://oeis.org/A009461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A009461
 *  Formula: a(n) = Σ_{k=0..n} (-1)^{n-k} binom(n,k) *
 *              Σ_{t≥0} (-1)^t * s(k,2t+1)
 *  where s(k,m) are the signed Stirling numbers of the first kind.
 *  This implements the EGF expansion of sin(log(1+x))/exp(x).
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* local static state for caching and memoisation */
static idx_t cache_size   = 0;    /* largest n allocated */
static idx_t computed_max = 0;    /* largest n computed */
static seq_t *A            = NULL;   /* a'_n = Σ (-1)^t s(n,2t+1)   */
static seq_t *a_vals       = NULL;   /* final a(n) values            */
static seq_t *s_prev       = NULL;   /* Stirling numbers row for n-1  */
static seq_t *s_curr       = NULL;   /* Stirling numbers row for n    */

/* expand the cached arrays to at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *newA     = alloc_seq_array(new_size);
    seq_t *newval   = alloc_seq_array(new_size);
    seq_t *newsp    = alloc_seq_array(new_size);
    seq_t *newsc    = alloc_seq_array(new_size);
    if (!newA || !newval || !newsp || !newsc)
        return;   /* allocation failed – caller will still return 0 */

    if (A) {   /* copy existing data */
        for (size_t i = 0; i <= cache_size; ++i) {
            newA[i]     = A[i];
            newval[i]   = a_vals[i];
            newsp[i]    = s_prev[i];
            newsc[i]    = s_curr[i];
        }
        free_seq_array(A);
        free_seq_array(a_vals);
        free_seq_array(s_prev);
        free_seq_array(s_curr);
    }

    /* initialise unused elements to zero */
    for (size_t i = cache_size + 1; i < new_size; ++i) {
        newA[i]     = 0;
        newval[i]   = 0;
        newsp[i]    = 0;
        newsc[i]    = 0;
    }

    A     = newA;
    a_vals = newval;
    s_prev = newsp;
    s_curr = newsc;
    cache_size = n;
}

/* initialise base values for n = 0 */
static void init_base(void)
{
    if (A)     return;   /* already initialised */
    size_t sz  = 1;
    A     = alloc_seq_array(sz);
    a_vals = alloc_seq_array(sz);
    s_prev = alloc_seq_array(sz);
    s_curr = alloc_seq_array(sz);
    if (!A || !a_vals || !s_prev || !s_curr) return;

    A[0]          = 0;   /* a'_0 = 0 */
    a_vals[0]     = 0;   /* a(0)  */
    s_prev[0]     = 1;   /* s(0,0) */
    cache_size    = 0;
    computed_max  = 0;
}

/* compute a(n) on demand, with memoisation */
seq_t a(idx_t n)
{
    init_base();              /* ensure at least the base case */
    ensure_cache(n);          /* grow storage if needed */

    /* compute all rows up to n if not yet done */
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        /* ---- Stirling numbers of the first kind for n = m ---- */
        /*   s(m,0) = 0 for m>0  */
        s_curr[0] = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t left  = s_prev[(size_t)(k - 1)];
            seq_t right = (seq_t)(m - 1) * s_prev[(size_t)k];
            s_curr[(size_t)k] = left - right;
        }

        /* ---- compute a'_m = Σ_{t} (-1)^t s(m,2t+1) ---- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; k += 2) {       /* odd k only */
            idx_t t     = (k - 1) >> 1;
            if ((t & 1) == 0)
                sum += s_curr[(size_t)k];
            else
                sum -= s_curr[(size_t)k];
        }
        A[(size_t)m] = sum;

        /* ---- convolution with e^{-x} (binomial transform) ---- */
        seq_t binom = 1;   /* binom(m,0) */
        seq_t an    = 0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = A[(size_t)k];
            if (((m - k) & 1) != 0)
                term = -term;   /* multiply by (-1)^{m-k} */
            an += binom * term;
            if (k < m) {
                /* binom(n,k+1) = binom(n,k) * (n-k)/(k+1)  */
                binom = (binom * (m - k)) / (k + 1);
            }
        }
        a_vals[(size_t)m] = an;

        /* prepare for next iteration: swap rows */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    computed_max = n;
    return a_vals[(size_t)n];
}
