/*
 * A198712  Number of nX5 0..3 arrays with values 0..3 introduced in row major
 *          order and no element equal to any horizontal or vertical neighbor.
 * https://oeis.org/A198712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 119*a(n‑1) – 2929*a(n‑2) + 25066*a(n‑3) – 76115*a(n‑4) *
 *            – 887*a(n‑5) + 324153*a(n‑6) – 444798*a(n‑7) + 175392*a(n‑8) *
 *            Recurrence initial terms: * a(1)=14, a(2)=1201, a(3)=107042,
 *            a(4)=9548295, * a(5)=851787199, a(6)=75987485516,
 *            a(7)=6778819400772, * a(8)=604736581320925
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* (seq_t, idx_t) are supplied by the build. */
seq_t a(idx_t n)
{
    /* Ensure n is at least 1 – the sequence is defined for n ≥ 1.  */
    if (n == 0) return (seq_t)0;

    /* Initial terms and coefficients of the 8‑term recurrence.  */
    enum { K = 8 };
    static const seq_t init[K] = {
        (seq_t)14ULL,
        (seq_t)1201ULL,
        (seq_t)107042ULL,
        (seq_t)9548295ULL,
        (seq_t)851787199ULL,
        (seq_t)75987485516ULL,
        (seq_t)6778819400772ULL,
        (seq_t)604736581320925ULL
    };
    static const int coeff[K] = {
        119,
        -2929,
        25066,
        -76115,
        -887,
        324153,
        -444798,
        175392
    };

    /* A static cache that grows as needed.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Grow the cache to hold at least n terms.  */
    if (n > cache_cap) {
        size_t new_cap = cache_cap == 0 ? K : cache_cap;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term is already cached, return it.  */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Compute terms up to n, extending the cache.  */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        seq_t value;
        if (k <= (size_t)K) {
            /* Use the hard‑coded initial terms.  */
            value = init[k - 1];
        } else {
            /* Use the recurrence: a(k) = Σ coeff[j]*a(k-1-j).  */
            seq_t sum = 0;
            for (int j = 0; j < K; ++j) {
                int c    = coeff[j];
                seq_t aij = cache[k - 2 - (size_t)j];
                seq_t prod = aij * (seq_t)(c >= 0 ? c : -c);
                sum += (c >= 0) ? prod : -prod;
            }
            value = sum;
        }
        cache[cache_size++] = value;
    }

    return cache[n - 1];
}
