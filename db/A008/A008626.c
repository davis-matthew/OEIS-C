/*
 * A008626  Poincaré series [or Poincare series] (or Molien series) for
 *          H*(M_11, GF(3)) and H*(M_23, GF(3)).
 * https://oeis.org/A008626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is given by the generating function
 *     G(x) = (1 -3x^7 +3x^8 -2x^15 +x^16-2x -3x^9 +3x^2 +3x^14 -4x^3 +4x^4 -4x^5 +4x^6 +4x^10 -4x^11 +4x^12 -4x^13)
 *     / ((1 +x^4)(x^8 +1)(x-1)^2(x^2 +1)^2).
 * This yields a linear recurrence of order 18:
 *     a(n) = -∑_{k=1}^{18} d_k a(n-k)   for n>16,
 * where the coefficients d_k are
 *     d = {1,-2,3,-4,4,-4,4,-4,4,-4,4,-4,4,-4,4,-4,3,-2,1}.
 * The values for n≤16 are obtained from
 *     a(n) = n_n - ∑_{k=1}^{n} d_k a(n-k),
 * where n_n are the coefficients of the numerator:
 *     n = {1,-2,3,-4,4,-4,4,-3,3,-3,4,-4,4,-4,3,-2,1}.
 */

#include <stdlib.h>
#include <stddef.h>

/* signed intermediate type matching the width of seq_t */
#if defined(__SIZEOF_INT128__)
typedef __int128 signed_seq_t;
#else
typedef long long signed_seq_t;
#endif

/* recurrence coefficients (d0 = 1) */
static const int d_coeff[19] = {
    1,-2,3,-4,4,-4,4,-4,4,-4,4,-4,4,-4,4,-4,3,-2,1
};

/* numerator coefficients n_0 … n_16 */
static const int n_coeff[17] = {
    1,-2,3,-4,4,-4,4,-3,3,-3,4,-4,4,-4,3,-2,1
};

static seq_t *cache = NULL;
static size_t cache_sz = 0;

/* ensure cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_sz > n) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    for (idx_t i = cache_sz; i <= n; ++i)
    {
        if (i <= 16)   /* inhomogeneous part */
        {
            signed_seq_t sum_terms = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum_terms += (signed_seq_t)d_coeff[k] *
                             (signed_seq_t)cache[i - k];

            signed_seq_t res = (signed_seq_t)n_coeff[i] - sum_terms;
            cache[i] = (seq_t)res;
        }
        else          /* homogeneous recurrence */
        {
            signed_seq_t sum_terms = 0;
            for (idx_t k = 1; k <= 18 && k <= i; ++k)
                sum_terms += (signed_seq_t)d_coeff[k] *
                             (signed_seq_t)cache[i - k];
            cache[i] = (seq_t)(-sum_terms);
        }
    }

    cache_sz = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
