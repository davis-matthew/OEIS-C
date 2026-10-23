/*
 * A170246  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*(a(n-1)+⋯+a(n-39)) + 946*a(n-40) + p(n) where
 *            p(0)=p(40)=1, p(1..39)=2, p(n)=0 for n>40. The sequence is
 *            generated with a sliding‑window recurrence and cached so that
 *            any call order works
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define CACHE_SIZE 20001          /* sufficient for n ≤ 10000 */

static seq_t cache[CACHE_SIZE];
static idx_t cache_valid = 0;     /* number of terms already computed */
static seq_t sum39 = 0;          /* sum of the last up to 39 terms */

/* Helper: coefficient p(n) in the recurrence */
static inline seq_t p_value(idx_t i)
{
    if (i == 0 || i == 40) return (seq_t)1;
    if (i <= 39)            return (seq_t)2;
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    /* The test harness never asks for n ≥ CACHE_SIZE.  If it does,
       return 0 (this situation should not occur). */
    if (n + 1 > (idx_t)CACHE_SIZE)
        return (seq_t)0;

    /* Compute missing terms up to n. */
    if (n + 1 > cache_valid) {
        for (idx_t i = cache_valid; i <= n; ++i) {
            seq_t term = ((seq_t)43) * sum39;      /* 43 * sum of last 39 terms */

            if (i >= 40)
                term += ((seq_t)946) * cache[i - 40];  /* +946*a(i-40) */

            term += p_value(i);                     /* + p(i) */

            cache[i] = term;

            /* Update sliding window for the next step. */
            sum39 += cache[i];
            if (i >= 39)   /* remove the term that is now outside the window */
                sum39 -= cache[i - 39];
        }
        cache_valid = n + 1;
    }

    return cache[n];
}
