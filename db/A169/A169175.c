/*
 * A169175  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169175: Number of reduced words of length n in the Coxeter group
 * on 18 generators with relations S_i² = (S_i S_j)²⁷ = I.
 *
 * The sequence is defined by the rational generating function
 *
 *     G(t) = (t²⁷ + 2t²⁶ + … + 2t + 1) /
 *            (1 - 16t - 16t² - … - 16t²⁶ + 136t²⁷)
 *
 * This yields the linear recurrence
 *
 *   a(0) = 1
 *   a(n) = 16*Σ_{k=max(0,n-26)}^{n-1} a(k)
 *          – (n≥27 ? 136*a(n-27) : 0)
 *          + (n==27 ? 1 : 2)      for n ≥ 1
 *
 * The function below uses a static on‑demand cache so that calls may
 * arrive in any order while staying <2 s for n≈10 000.
 */

#include <stdlib.h>

/* The types seq_t (arbitrary‑precision or native integer) and idx_t
 * (usually uint64_t) are provided by the build environment. */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of computed entries   */
    static idx_t cache_cap   = 0;   /* allocated capacity (in entries) */

    /* Allocate / grow the cache if needed */
    if (cache_cap <= n) {
        idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – return zero as a
                               placeholder (sequence never asks for n < 0) */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;                 /* a(0) = 1 */
        } else {
            /* Sum a(k) for k = max(0,i-26) .. i-1  */
            seq_t sum = 0;
            idx_t start = (i > 26) ? (i - 26) : 0;
            for (idx_t j = start; j < i; ++j) {
                sum += cache[j];
            }
            sum *= 16;                 /* 16 * Σ a(k) */

            /* Subtract 136*a(i-27) if applicable */
            if (i >= 27) {
                sum -= 136 * cache[i - 27];
            }

            /* Add the shortcut coefficient b(i) */
            if (i == 27) {
                sum += 1;
            } else {
                sum += 2;              /* 2 for 1 ≤ i ≤ 26 */
            }

            val = sum;
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
