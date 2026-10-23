/*
 * A009618  Expansion of sinh(x)*cos(log(1+x)).
 * https://oeis.org/A009618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k odd, 1≤k≤n} binom(n,k) * T(n-k) where T(r) = Σ_{m≥0}
 *            (-1)^m * s(r,2m) and s(r,k) are the signed Stirling numbers of
 *            the first kind. Recurrences: T(0) = 1, O(0) = 0 T(r) = -O(r-1) -
 *            (r-1) * T(r-1) O(r) = T(r-1) - (r-1) * O(r-1) Here O(r) = Σ_{k
 *            odd} (-1)^{(k-1)/2} * s(r,k). The arrays...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *T_cache = NULL;   /* Σ_{m} (-1)^m s(n,2m)   */
static seq_t *O_cache = NULL;   /* Σ_{m} (-1)^{m} s(n,2m-1) */
static idx_t  cache_max = 0;    /* highest n for which T and O are valid */

/* Ensure T_cache[0..n] and O_cache[0..n] are computed */
static void ensure_T_O(idx_t n)
{
    if (cache_max >= n) return;

    size_t new_size = (size_t)n + 1;
    T_cache = (seq_t *)realloc(T_cache, new_size * sizeof(seq_t));
    O_cache = (seq_t *)realloc(O_cache, new_size * sizeof(seq_t));

    /* If this is the first allocation, initialise the base case */
    if (cache_max == 0) {
        T_cache[0] = 1;   /* s(0,0)=1  */
        O_cache[0] = 0;   /* no odd terms for n=0 */
    }

    for (idx_t r = cache_max + 1; r <= n; ++r) {
        seq_t termT = -O_cache[r-1];
        termT += - (seq_t)(r-1) * T_cache[r-1];
        T_cache[r] = termT;

        seq_t termO =  T_cache[r-1];
        termO += - (seq_t)(r-1) * O_cache[r-1];
        O_cache[r] = termO;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Prepare T coefficients up to n */
    ensure_T_O(n);

    seq_t sum   = 0;  /* accumulation of a(n) */
    seq_t binom = 1;  /* binom(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binom to binom(n,k) */
        binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;

        if (k & 1) {          /* only odd k contribute */
            sum += binom * T_cache[n - k];
        }
    }
    return sum;
}
