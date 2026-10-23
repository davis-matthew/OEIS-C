/*
 * A109883  Start subtracting from n its divisors beginning from 1 until one
 *          reaches a number smaller than the last divisor subtracted or
 *          reaches the last nontrivial divisor < n. Define this to be the
 *          perfect deficiency of n. Then a(n) = perfect deficiency of n.
 * https://oeis.org/A109883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = perfect deficiency obtained by subtracting the proper
 *            divisors of n in increasing order until the remaining value is
 *            less than the last divisor subtracted or the last non‑trivial
 *            divisor has been subtracted
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the harness. */
#include <stdlib.h>
#include <math.h>
#include "../utils/cmp_idx.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* Special case */

    /* Allocate space for all proper divisors < n (≤ 2*sqrt(n)) */
    size_t cap = 2 * (size_t) sqrt((double) n) + 10;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return (seq_t)0;     /* allocation failure – unlikely */

    size_t dcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (d < n && dcnt < cap) divs[dcnt++] = d;
            idx_t other = n / d;
            if (other != d && other < n && dcnt < cap)
                divs[dcnt++] = other;
        }
    }

    /* Sort the divisors in ascending order */
    qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

    seq_t cur = (seq_t) n;          /* Current value to reduce */

    for (size_t i = 0; i < dcnt; ++i)
    {
        seq_t d = (seq_t) divs[i];
        if (cur < d) break;         /* Stop if resulting value would be negative */
        cur -= d;
        if (i == dcnt - 1) break;   /* Reached the last non‑trivial divisor */
    }

    free(divs);
    return cur;
}
