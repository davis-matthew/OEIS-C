/*
 * A117226  Number of permutations of [n] avoiding the consecutive pattern
 *          1243.
 * https://oeis.org/A117226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117226 – number of permutations of [n] avoiding the consecutive pattern 1243
   Recurrence (valid for n ≥ 1):
     a(0) = 1
     a(n) = Σ_{m = 0..⌊(n-1)/3⌋} (-3)^m * (1/3)_m *
            binomial(n, 3m+1) * a(n-3m-1)
   where (1/3)_m = Π_{k=0}^{m-1} (1/3 + k) and
   (-3)^m * (1/3)_m = (-1)^m * Π_{j=1}^m (3j-2).
   The implementation uses the integer product Π_{j=1}^m (3j-2) (stored in
   weight[m]) and treats the sign separately.
*/

#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>

/* Ensure dynamic array with capacity ≥ new_cap */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap >= new_cap)
        return;
    size_t new_size = (*cap) ? (*cap) : 2;
    while (new_size < new_cap)
        new_size <<= 1;
    seq_t *ptr = (seq_t *)realloc(*arr, new_size * sizeof(seq_t));
    *arr = ptr;
    *cap = new_size;
}

/* Global caches */
static seq_t *a_cache      = NULL;
static size_t a_cap = 0;   /* total allocated slots */
static size_t a_count = 0; /* number of computed terms (a_cache[0..a_count-1]) */

static seq_t *weight      = NULL;
static size_t w_cap = 0;   /* total allocated slots */
static size_t w_count = 0; /* number of computed weight terms (weight[0..w_count-1]) */

/* Compute the weight product Π_{j=1}^m (3j-2) (only positive part) */
static void ensure_weight(size_t needed)
{
    if (w_count > needed)
        return;
    ensure_capacity(&weight, &w_cap, needed + 1);      /* +1 because we keep weight[0] */
    if (w_count == 0) {
        weight[0] = (seq_t)1;   /* product over empty set */
        w_count = 1;
    }
    for (size_t i = w_count; i <= needed; ++i) {
        weight[i] = weight[i - 1] * (seq_t)(3 * (idx_t)i - 2);
    }
    w_count = needed + 1;
}

/* Ensure a_cache can hold terms up to n (inclusive) */
static void ensure_a_cache(idx_t n)
{
    if (a_count == 0) {
        /* Initialize first term a(0) = 1 */
        ensure_capacity(&a_cache, &a_cap, 1);
        a_cache[0] = (seq_t)1;
        a_count = 1;
    }
    if ((size_t)n >= a_count) {
        ensure_capacity(&a_cache, &a_cap, (size_t)n + 1);
        /* compute missing terms */
        for (idx_t i = a_count; i <= n; ++i) {
            idx_t max_m = (i - 1) / 3;
            /* weight[max_m] will be needed later */
            ensure_weight(max_m);

            /* binomial C(i, 3m+1) – start with k=1 i.e. C(i,1) = i */
            seq_t bin = (seq_t)i;
            idx_t k = 1;

            /* sum over m, keeping a separate sign flag to avoid negatives */
            seq_t sum_val = 0;
            bool sum_neg = false;            /* false → positive, true → negative */

            for (idx_t m = 0; m <= max_m; ++m) {
                /* term = weight[m] * bin * a[i-3m-1] with sign (-1)^m */
                seq_t term = weight[m] * bin;
                term *= a_cache[(size_t)(i - 3 * m - 1)];
                bool term_neg = (m & 1U) != 0;      /* sign of this term */

                /* signed addition: sum_val (+/-) term → sum_val with new sign */
                if (sum_neg == term_neg) {
                    sum_val += term;
                } else {
                    if (sum_val >= term) {
                        sum_val -= term;
                    } else {
                        seq_t tmp = term - sum_val;
                        sum_val = tmp;
                        sum_neg = term_neg;
                    }
                }

                /* Prepare next binomial C(i, current_k+3) if needed */
                if (m < max_m) {
                    for (idx_t step = 0; step < 3; ++step) {
                        idx_t r = k + step;
                        /* bin = bin * (i - r) / (r + 1) */
                        seq_t num = (seq_t)(i - r);
                        seq_t den = (seq_t)(r + 1);
                        bin = (bin * num) / den;
                    }
                    k += 3;
                }
            }
            a_cache[(size_t)i] = sum_val;
        }
        a_count = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    ensure_a_cache(n);
    return a_cache[(size_t)n];
}
