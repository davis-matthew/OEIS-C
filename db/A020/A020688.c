/*
 * A020688  Numbers of form 5 x^2 + 9 y^2.
 * https://oeis.org/A020688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by exhaustive generation: a(n) is the nth smallest non‑negative
   integer that can be written as 5*x^2 + 9*y^2 (x,y ≥ 0). This routine
   generates all such values up to a dynamic bound, sorts and deduplicates
   them, and returns the required term. */
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for unsigned long long values used by qsort */
static int cmp_ull(const void *p1, const void *p2)
{
    const unsigned long long *a = (const unsigned long long *)p1;
    const unsigned long long *b = (const unsigned long long *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                           /* undefined indices irrelevant */

    /* Initial bound: roughly thrice the desired index to guarantee enough terms */
    unsigned long long bound = n * 3ULL + 100ULL;

    while (1)
    {
        /* Dynamic array to hold candidate values (as ULL to avoid overflow) */
        unsigned long long *vals = NULL;
        size_t cap = 0, count = 0;

        /* Generate all 5*x^2 + 9*y^2 ≤ bound */
        for (unsigned long long y = 0; ; ++y)
        {
            unsigned long long y2   = y * y;
            unsigned long long termY = 9ULL * y2;
            if (termY > bound) break;

            for (unsigned long long x = 0; ; ++x)
            {
                unsigned long long x2 = x * x;
                unsigned long long val = 5ULL * x2 + termY;
                if (val > bound) break;

                if (count == cap)
                {
                    size_t new_cap = (cap == 0) ? 1024 : cap * 2;
                    unsigned long long *tmp =
                        (unsigned long long *)realloc(vals, new_cap * sizeof(unsigned long long));
                    if (!tmp) { free(vals); return 0; } /* abort on allocation failure */
                    vals = tmp;
                    cap = new_cap;
                }
                vals[count++] = val;
            }
        }

        /* Sort the collected values */
        qsort(vals, count, sizeof(unsigned long long), cmp_ull);

        /* Remove duplicates in place */
        size_t uniq = 0;
        unsigned long long prev = 0;
        for (size_t i = 0; i < count; ++i)
        {
            if (i == 0 || vals[i] != prev)
            {
                vals[uniq++] = vals[i];
                prev = vals[i];
            }
        }

        if (uniq >= n)
        {
            unsigned long long result = vals[n - 1];
            free(vals);
            return (seq_t)result;
        }

        free(vals);
        /* Not enough terms: enlarge the search bound and repeat */
        bound += bound / 2 + 100ULL;
    }
}
