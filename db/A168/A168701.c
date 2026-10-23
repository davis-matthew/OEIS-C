/*
 * A168701  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168701: number of reduced words of length n in the Coxeter
   group on 24 generators with (S_i)^2 = (S_i S_j)^17 = I.
   The generating function is
        (t^17 + 2*t^16 + 2*t^15 + ... + 2*t + 1) /
        (1 - 22*t - 22*t^2 - ... - 22*t^16 + 253*t^17).
   Thus the coefficients satisfy the linear recurrence
        a(n) = 22*a(n-1) + 22*a(n-2) + ... + 22*a(n-16) - 253*a(n-17)
        for n >= 17,
   with the inhomogeneous terms
        p(0)=1,  p(1..16)=2,  p(17)=1,  p(n)=0 for n>=18.
   Initial condition: a(0)=1.
   The function below memoises the terms so that it works
   for arbitrary order of calls and for n ≤ ~10^5 comfortably. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i < cached_len */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;

    /* Ensure capacity for n+1 elements */
    if (n + 1 > cached_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 512)
            new_cap = 512;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In production code we would handle realloc failure */
        cached_cap = new_cap;
    }

    /* Compute terms up to n if necessary */
    if (cached_len <= (size_t)n) {
        size_t idx;
        /* Initialise a(0) if not already */
        if (cached_len == 0) {
            cache[0] = (seq_t)1;
            cached_len = 1;
        }
        for (idx = cached_len; idx <= (size_t)n; ++idx) {
            seq_t term = 0;
            /* Sum of 22 * a(idx - k) for k = 1 .. 16, if idx >= k */
            size_t k;
            for (k = 1; k <= 16; ++k) {
                if (idx >= k) {
                    term += (seq_t)22 * cache[idx - k];
                }
            }
            /* Subtract 253 * a(idx - 17) if idx >= 17 */
            if (idx >= 17) {
                term -= (seq_t)253 * cache[idx - 17];
            }
            /* Add the inhomogeneous part p(idx) */
            seq_t p;
            if (idx == 0) {
                p = (seq_t)1;
            } else if (idx <= 16) {
                p = (seq_t)2;
            } else if (idx == 17) {
                p = (seq_t)1;
            } else {
                p = (seq_t)0;
            }
            term += p;
            cache[idx] = term;
        }
        cached_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
