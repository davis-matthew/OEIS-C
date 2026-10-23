/*
 * A151747  Except for boundary cases (n <= 3, j = 0, 1, 2^i-1), satisfies
 *          a(n) = a(2^i+j) = 2 a(j) + a(j+1), where n = 2^i + j, 0 <= j < 2^i
 *          .
 * https://oeis.org/A151747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A151747
   Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=5.
   For n ≥ 4 let i = ⌊log₂ n⌋ and base = 2ᶦ, j = n – base.
   The recurrence (valid for all 0 < j < base‑1) is
            a(n) = 2·a(j) + a(j+1)
   Boundary cases:
            a(n) = 2n – 1               for n ≤ 3
            a(2ᶦ)      = 1 + (3i+1)·2ᶦ⁻²   for i ≥ 2
            a(2ᶦ + 1)  = 3 + 3·2ᶦ⁻¹      for i ≥ 2
            a(2ᶦ + (2ᶦ – 1)) = 2·a(2ᶦ⁻¹ + (2ᶦ⁻¹ – 1)) + a(2ᶦ) – 1
   The implementation memoises computed values so that calls
   may be made in any order.
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;     /* number of cached entries (cache_len == 0 => empty) */

/* floor(log₂(v)) for v > 0 */
static inline idx_t floor_log2(idx_t v)
{
    /* builtin_clzll defined for unsigned long long, which is 64‑bit */
    return 63 - __builtin_clzll(v);
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Grow cache so that indices 0 … n are available */
    if ((idx_t)n + 1 > cache_len)
    {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute values from the first unknown index up to n */
        for (idx_t k = cache_len; k <= n; ++k)
        {
            seq_t val;
            if (k == 0)
                val = 0;
            else if (k <= 3)
                val = 2 * (seq_t)k - 1;
            else
            {
                idx_t i = floor_log2(k);                 /* i ≥ 2 for k ≥ 4 */
                idx_t base = ((idx_t)1) << i;           /* base = 2ᶦ */
                idx_t j = k - base;

                if (j == 0)                           /* k = 2ᶦ, i ≥ 2 */
                {
                    val = 1 + ((seq_t)(3 * i + 1)) * (((seq_t)1) << (i - 2));
                }
                else if (j == 1)                      /* k = 2ᶦ+1, i ≥ 2 */
                {
                    val = 3 + 3 * (((seq_t)1) << (i - 1));
                }
                else if (j == base - 1)               /* boundary j = 2ᶦ-1 */
                {
                    /* val = 2*a(j) + a(j+1) - 1   (a(j+1) = a(2ᶦ) computed earlier) */
                    val = 2 * cache[j] + cache[j + 1] - 1;
                }
                else                                   /* ordinary recurrence */
                {
                    val = 2 * cache[j] + cache[j + 1];
                }
            }
            cache[k] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
