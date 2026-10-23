/*
 * A170387  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (t^43 + 2*t^42 + ... + 2*t + 1)/(1 - 40*t - 40*t^2 - ...
 *            - 40*t^42 + 820*t^43); Recurrence for n ≥ 43: a(n) = 40 *
 *            (a(n-1)+...+a(n-42)) - 820 * a(n-43)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed values. */
    static seq_t *cache     = NULL;   /* sequence storage */
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t cache_len = 0;      /* number of terms already computed */

    /* Allocate enough space for requested index. */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the first 44 terms once using the rational generating function. */
    if (cache_len < 44) {
        /* Ensure capacity for 44 terms. */
        if (cache_cap < 44) {
            size_t new_cap = 64;
            while (new_cap < 44) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = 0; i < 44; ++i) {
            seq_t sum = 0;
            size_t maxk = i < 43 ? (size_t)i : 43;   /* highest k that satisfies k ≤ i */

            for (size_t k = 1; k <= maxk; ++k) {
                int coeff = (k == 43) ? 820 : -40;    /* coefficient of t^k in the denominator */
                sum += ((seq_t)coeff) * cache[i - k];
            }

            seq_t Pn = (i == 0 || i == 43) ? (seq_t)1 : (seq_t)2;   /* coefficient of t^i in the numerator */
            cache[i] = Pn - sum;
        }

        cache_len = 44;
    }

    /* Compute further terms using the linear recurrence. */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            for (int k = 1; k <= 42; ++k) sum += cache[i - k];
            sum = sum * ((seq_t)40);
            cache[i] = sum - ((seq_t)820) * cache[i - 43];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
