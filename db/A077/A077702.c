/*
 * A077702  a(1) = 5; a(n) is the smallest multiple of a(n-1) that contains
 *          all the digits of a(n-1) and is not a multiple of 10.
 * https://oeis.org/A077702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=5; a(n)=smallest multiple of a(n-1) that contains each
 *            digit of a(n‑1) with at least the same multiplicity and is not a
 *            multiple of 10
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   digit_freq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_freq.h"

static seq_t *cache = NULL;      /* array of terms (0‑based) */
static idx_t  cache_len = 0;     /* number of terms stored (>=0) */
static idx_t  cache_cap = 0;     /* allocated capacity */

/* enlarge cache if needed */
static void ensure_capacity (idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* sequence value */
seq_t a (idx_t n)
{
    if (n < 1) return 0;                 /* undefined, but no caller should request */

    if (n > cache_len)
    {
        ensure_capacity(n);
        for (idx_t i = cache_len; i < n; ++i)
        {
            if (i == 0)                 /* a(1) = 5 */
            {
                cache[0] = 5;
                continue;
            }

            seq_t prev = cache[i-1];
            unsigned int prevFreq[10];
            digit_freq(prev, prevFreq);

            seq_t cand = 0;
            for (idx_t k = 3; ; k += 2)          /* odd multipliers only (prev ends with 5) */
            {
                cand = prev * (seq_t)k;
                unsigned int candFreq[10];
                digit_freq(cand, candFreq);

                bool ok = true;
                for (int d = 0; d < 10; ++d)
                {
                    if (candFreq[d] < prevFreq[d]) { ok = false; break; }
                }
                if (ok) { cache[i] = cand; break; }
            }
        }
        cache_len = n;
    }
    return cache[n-1];
}
