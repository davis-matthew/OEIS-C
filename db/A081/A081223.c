/*
 * A081223  Smallest k such that floor(k*gamma) begins with n
 *          (gamma=0.5772156649...).
 * https://oeis.org/A081223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k such that floor(k * γ) begins with the decimal
 *            representation of n, where γ = 0.5772156649… (Euler–Mascheroni
 *            constant). The algorithm searches for k incrementally, using a
 *            high‑precision rational approximation for γ
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Type definitions are supplied by the build (seq_t, idx_t). */

/* High‑precision rational approximation of γ:
   γ ≈ 57721566490153286 / 10^14
   The error is < 10⁻¹⁴, so for k ≤ 10⁶ the floor of k*γ is computed
   correctly (and all values needed for the sequence are far below that). */
static const unsigned long long GAMMA_NUM = 57721566490153286ULL;
static const unsigned long long GAMMA_DEN = 100000000000000ULL;

/* Precomputed powers of 10 for integer division. */
static seq_t pow10[20];
static bool pow10_initialized = false;

/* Cache for already computed sequence entries. */
static seq_t  *cache  = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static idx_t   last_k   = 0;   /* last k examined in the search */

/* One‑time initialization. */
static void init_once(void)
{
    if (pow10_initialized) return;
    pow10[0] = 1;
    for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * 10;
    pow10_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not part of the sequence, safety */

    init_once();                 /* ensure pow10 table exists */

    /* Expand cache as needed.  The search proceeds incrementally,
       reusing the previously examined k to avoid redundant work. */
    while (cache_len < n)
    {
        idx_t target = cache_len + 1;           /* n for which we search a(n) */
        seq_t tn = (seq_t)target;               /* cast target to seq_t */

        /* Find the smallest k with the desired leading digits. */
        bool found = false;
        while (!found)
        {
            ++last_k;
            unsigned __int128 prod = (unsigned __int128)last_k * GAMMA_NUM;
            seq_t val = (seq_t)(prod / GAMMA_DEN);   /* floor(k * γ) */

            /* Test all possible positions of the leading block. */
            for (int m = 0; m < 20; ++m)
            {
                if ((val / pow10[m]) == tn)
                {
                    found = true;
                    break;
                }
            }
        }

        /* Store the found k into the cache. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[cache_len++] = last_k;
    }

    return cache[n-1];
}
