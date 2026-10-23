/*
 * A023987  Sum of exponents of primes in C(5n,3n) - sum of exponents of
 *          primes in C(3n,2n).
 * https://oeis.org/A023987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A022559(5*n) - 2*A022559(3*n) + A022559(n)
 * Range:     u8 n <= 1524, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Sum of exponents of primes in C(5n,3n) - sum of exponents of primes in C(3n,2n) */

#include <stdlib.h>
#include "../utils/bigomega.h"

/* Cached cumulative sums of A022559(k) up to the current maximum k */
static seq_t *cum = NULL;
static idx_t  cum_len = 0;          /* number of elements in cum (cum_len-1 = last index) */

/* Ensure that cum array has entries computed up to at least k */
static void ensure_cum(idx_t k)
{
    if (cum_len == 0) {
        /* Create array with at least k+1 elements */
        size_t new_len = (size_t)k + 1;
        seq_t *tmp = (seq_t *)realloc(cum, new_len * sizeof(seq_t));
        if (!tmp) abort();           /* memory allocation failed */
        cum = tmp;
        cum[0] = 0;                  /* A022559(0) = 0 */
        cum_len = 1;
    }

    if (cum_len <= k) {
        size_t old_len = (size_t)cum_len;
        size_t new_len = (size_t)k + 1;
        seq_t *tmp = (seq_t *)realloc(cum, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cum = tmp;
        for (idx_t i = old_len; i <= k; ++i) {
            seq_t w = bigomega((idx_t)i);          /* total prime factors of i */
            cum[i] = cum[i-1] + w;                  /* cumulative sum for i */
        }
        cum_len = new_len;
    }
}

seq_t a(idx_t n)
{
    idx_t k5 = 5 * n;
    idx_t k3 = 3 * n;

    /* Ensure cumulative arrays are computed up to the necessary values */
    ensure_cum(k5);

    /* Fetch required cumulative sums */
    seq_t five_n  = cum[k5];
    seq_t three_n = cum[k3];
    seq_t one_n   = cum[n];

    /* Compute a(n) = A022559(5n) - 2*A022559(3n) + A022559(n) */
    seq_t res = five_n - ((seq_t)2 * three_n) + one_n;
    return res;
}
