/*
 * A117824  a(0) = 0, a(1) = 1; for n >= 2, a(n) = a(n-1) + a(n-2) - (n-1) if
 *          that number is positive and not already in the sequence, otherwise
 *          a(n) = a(n-1) + a(n-2) + (n-1).
 * https://oeis.org/A117824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n≥2,
   let s = a[n‑1] + a[n‑2].
   If s‑(n‑1) > 0 and not yet in the sequence, a[n] = s‑(n‑1);
   otherwise a[n] = s+(n‑1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size (number of slots)   */
    static size_t len   = 0;   /* number of terms actually filled     */

    /* Convert n to a size_t for array indexing – abort if too big. */
    if (n >= (idx_t)SIZE_MAX) abort();          /* cannot allocate that many terms */
    size_t needed = (size_t)n + 1;              /* we need indices 0 … n */

    /* Grow the cache if necessary. */
    if (cap < needed)
    {
        size_t new_cap = (cap == 0 ? 8 : cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms on demand up to n. */
    if (len < needed)
    {
        for (size_t i = len; i < needed; ++i)
        {
            if (i == 0) { cache[i] = (seq_t)0; continue; }
            if (i == 1) { cache[i] = (seq_t)1; continue; }

            /* i >= 2 : compute using the recurrence. */
            seq_t sum = cache[i-1] + cache[i-2];
            seq_t sub = (seq_t)(i-1);
            seq_t tentative;
            bool use_minus = false;

            if (sum > sub)
            {
                tentative = sum - sub;
                use_minus = true;
            }

            if (use_minus)
            {
                /* Check if tentative already occurs. */
                bool present = false;
                for (size_t j = 0; j < i; ++j)
                {
                    if (cache[j] == tentative)
                    {
                        present = true;
                        break;
                    }
                }
                cache[i] = present ? sum + sub : tentative;
            }
            else
            {
                cache[i] = sum + sub;
            }
        }
        len = needed;
    }

    return cache[n];
}
