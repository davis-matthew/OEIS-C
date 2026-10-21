/*
 * A238133  Difference between A238131(n) and A238132(n).
 * https://oeis.org/A238133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = ∑_{g≤n, g = k(3k−1)/2 for some k≥0} (−1)^(k mod 2) ·
 *            d(n−g) where d(m) is the divisor function. This is the
 *            convolution of the divisor function A000005 with the indicator
 *            function of the generalized pentagonal numbers, yielding the
 *            difference between the number of parts in all partitions o...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

/* Cache for divisor counts and sequence values */
static seq_t *D = NULL;          /* divisor counts d(i) */
static seq_t *A = NULL;          /* a(i) values */
static size_t mem_cap = 0;       /* current capacity of D and A */

/* Cache for generalized pentagonal numbers and their signs */
static idx_t *pent_vals = NULL;  /* pentagonal numbers g = k(3k−1)/2 */
static int   *pent_sgn  = NULL;  /* sign: (+1) for k even, (-1) for k odd */
static size_t pent_len  = 0;     /* number of stored pentagonal numbers */

/* Ensure D and A are at least 'cap' elements long; grow with seq_realloc. */
static void ensure_cache_size(size_t cap)
{
    if (mem_cap < cap) {
        D = seq_realloc(D, mem_cap, cap);
        A = seq_realloc(A, mem_cap, cap);
        mem_cap = cap;
    }
}

/* Recompute all data up to 'max_n' inclusive.  This is amortized O(n log n). */
static void recompute(idx_t max_n)
{
    /* 1. Allocate/resize D and A */
    ensure_cache_size((size_t)max_n + 1);

    /* 2. Build divisor counts d(i) by a divisor sieve. */
    for (idx_t i = 0; i <= max_n; ++i) D[i] = 0;
    for (idx_t d = 1; d <= max_n; ++d)
        for (idx_t m = d; m <= max_n; m += d)
            ++D[m];

    /* 3. Generate generalized pentagonal numbers up to max_n. */
    /* Start with modest capacity; grow on demand. */
    size_t pent_cap = max_n + 1;
    pent_vals = (idx_t *)realloc(pent_vals, pent_cap * sizeof(idx_t));
    pent_sgn  = (int  *)realloc(pent_sgn,  pent_cap * sizeof(int));
    if (!pent_vals || !pent_sgn) exit(1);

    pent_len = 0;
    /* k = 0 gives g = 0 with +1 sign */
    pent_vals[pent_len] = 0;
    pent_sgn[pent_len]  = 1;
    ++pent_len;

    for (idx_t k = 1; ; ++k) {
        /* g1 = k(3k−1)/2,  g2 = k(3k+1)/2 */
        __int128 g1 = (__int128)k * (3 * k - 1) / 2;
        __int128 g2 = (__int128)k * (3 * k + 1) / 2;
        if (g1 > max_n && g2 > max_n) break;

        if (g1 <= max_n) {
            if (pent_len == pent_cap) {
                pent_cap *= 2;
                pent_vals = (idx_t *)realloc(pent_vals, pent_cap * sizeof(idx_t));
                pent_sgn  = (int  *)realloc(pent_sgn,  pent_cap * sizeof(int));
                if (!pent_vals || !pent_sgn) exit(1);
            }
            pent_vals[pent_len] = (idx_t)g1;
            pent_sgn[pent_len]  = (k & 1) ? -1 : 1;
            ++pent_len;
        }
        if (g2 <= max_n) {
            if (pent_len == pent_cap) {
                pent_cap *= 2;
                pent_vals = (idx_t *)realloc(pent_vals, pent_cap * sizeof(idx_t));
                pent_sgn  = (int  *)realloc(pent_sgn,  pent_cap * sizeof(int));
                if (!pent_vals || !pent_sgn) exit(1);
            }
            pent_vals[pent_len] = (idx_t)g2;
            pent_sgn[pent_len]  = (k & 1) ? -1 : 1;
            ++pent_len;
        }
    }

    /* 4. Compute the sequence values a(0)..a(max_n). */
    for (idx_t n = 0; n <= max_n; ++n) {
        seq_t sum = 0;
        for (size_t i = 0; i < pent_len; ++i) {
            idx_t g = pent_vals[i];
            if (g > n) break;
            sum += (seq_t)pent_sgn[i] * D[n - g];
        }
        A[n] = sum;
    }
}

/* Public function implementing the sequence.  It lazily recomputes as needed. */
seq_t a(idx_t n)
{
    if ((size_t)n >= mem_cap)
        recompute(n);
    return A[n];
}
