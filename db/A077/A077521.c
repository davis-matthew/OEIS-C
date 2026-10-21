/*
 * A077521  a(1) = 1, a(n) = smallest squarefree number which begins with n
 *          and is a multiple of the previous term.
 * https://oeis.org/A077521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   is_squarefree, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/is_squarefree.h"

/* Recurrence: a(1) = 1;
   a(n) = smallest squarefree integer that starts with the decimal digits of n
   and is a multiple of a(n-1). */

#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static bool known[MAX_CACHE];

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for index 0, but harmless */

    /* Compute all terms up to n, if not already cached */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (known[i - 1])  /* already computed */
            continue;

        if (i == 1) {
            cache[0] = (seq_t)1;
            known[0] = true;
            continue;
        }

        seq_t prev  = cache[i - 2];
        seq_t value = 0;  /* will hold the found term */

        /* Search over increasing number of trailing digits (k = 0,1,2,…) */
        for (idx_t k = 0; ; ++k)
        {
            seq_t p10   = pow10(k);                             /* 10^k */
            seq_t low   = (seq_t)i * p10;                       /* n·10^k   */
            seq_t high  = ((seq_t)(i + 1)) * p10 - (seq_t)1;     /* (n+1)·10^k – 1 */

            /* First multiple of prev that is ≥ low */
            seq_t m     = (low + prev - (seq_t)1) / prev;
            seq_t cand  = m * prev;

            if (cand > high)
                continue;  /* this k gives no multiple within the interval */

            /* Scan all multiples of prev in [low, high] */
            while (cand <= high)
            {
                if (is_squarefree(cand))
                {
                    value = cand;   /* smallest candidate for this n */
                    break;
                }
                ++m;
                cand = m * prev;
            }

            if (value != (seq_t)0)
                break;  /* term found */
        }

        cache[i - 1] = value;
        known[i - 1] = true;
    }

    return cache[n - 1];
}
