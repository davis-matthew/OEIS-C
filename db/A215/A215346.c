/*
 * A215346  Expansion of (1/q) * phi(-q) * phi(q^4) / (phi(q) * psi(q^8)) in
 *          powers of q where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A215346
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^(n+1) in the product * ∏_{k≥1} (1 -
 *            q^k)^{e_k}, * where the exponent pattern e_k is periodic with
 *            period 16: * e_k = 4,-2,4,-2,4,-2,4,4,4,-2,4,-2,4,-2,4,0. * This
 *            product equals (η(q)^2 η(q^8)^3 / (η(q^2)^3 η(q^16)^2))^2, * and
 *            a(n) is the coefficient after multiplyin...
 * Range:     i8 n <= 5, i16 n <= 18, i32 n <= 66, i64 n <= 241, i128 n <= 898,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* allocate and free arrays that work in both C and C++ */
#ifdef __cplusplus
static seq_t* alloc_array(size_t n) { return new seq_t[n](); }
static void free_array(seq_t *p) { delete [] p; }
#else
static seq_t* alloc_array(size_t n) { return (seq_t*)calloc(n, sizeof(seq_t)); }
static void free_array(seq_t *p) { free(p); }
#endif

/* exponent pattern for (1 - q^k)^e_k */
static const int exp_pattern[16] =
{ 4, -2, 4, -2, 4, -2, 4, 4,
  4, -2, 4, -2, 4, -2, 4, 0 };

/* compute coefficients up to index 'limit' (inclusive) */
static seq_t* compute_series(idx_t limit)
{
    size_t L = (size_t)limit + 1;          /* array size = limit + 1   */
    seq_t *curr = alloc_array(L);
    seq_t *next = alloc_array(L);

    /* initialise coefficient of q^0 */
    curr[0] = 1;
    for (size_t i = 1; i < L; ++i) curr[i] = 0;

    for (size_t k = 1; k <= (size_t)limit; ++k)
    {
        int e = exp_pattern[(k - 1) & 15];

        if (e == 4)        /* (1 - q^k)^4 */
        {
            for (size_t n = 0; n < L; ++n)
            {
                seq_t val = curr[n];
                if (n >= k)         val -= (seq_t)4 * curr[n - k];
                if (n >= 2 * k)     val += (seq_t)6 * curr[n - 2 * k];
                if (n >= 3 * k)     val -= (seq_t)4 * curr[n - 3 * k];
                if (n >= 4 * k)     val += curr[n - 4 * k];
                next[n] = val;
            }
        }
        else if (e == -2)   /* (1 - q^k)^-2 */
        {
            for (size_t r = 0; r < k; ++r)
            {
                size_t maxT = (L - 1 > r) ? (L - 1 - r) / k : 0;
                seq_t S = 0;   /* Σ curr[r + m*k]          */
                seq_t M = 0;   /* Σ m * curr[r + m*k]     */
                for (size_t t = 0; t <= maxT; ++t)
                {
                    size_t idx = r + t * k;
                    seq_t cur = curr[idx];
                    S += cur;
                    M += (seq_t)t * cur;
                    next[idx] = (seq_t)(t + 1) * S - M;
                }
            }
        }
        else /* e == 0 : factor is 1 */
        {
            for (size_t n = 0; n < L; ++n) next[n] = curr[n];
        }

        /* swap arrays */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    free_array(next);                /* free the temporary buffer   */
    return curr;                     /* curr holds the final series */
}

/* static cache for memoisation */
static idx_t cached_limit = -1;
static seq_t *cached_series = NULL;

/* public function required by the test harness */
seq_t a(idx_t n)
{
    /* required coefficient index in H(q), which is n+1 because of 1/q */
    idx_t Q = n + 1;                        /* Q >= 0 for n >= -1   */

    if (cached_limit < Q)                  /* need to extend the cache   */
    {
        if (cached_series) free_array(cached_series);
        cached_series = compute_series(Q);
        cached_limit  = Q;
    }

    return cached_series[Q];
}
