/*
 * A168773  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in (t^18 + 2t^17 + … + 2t + 1) /
 *            (1081t^18 - 46t^17 - … - 46t + 1) Recurrence: a(n) = 46·(a(n‑1)+
 *            … + a(n‑17)) – 1081·a(n‑18) for n ≥ 18 Initial terms (n = 0 …
 *            17) are obtained from the generating function; they are
 *            incorporated automatically by the same recurrence with...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;   /* cache of already computed terms */
    static size_t len  = 0;      /* number of cached entries  */

    size_t target = (size_t)n;
    if (target >= len)          /* need to extend the cache */
    {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(vals, new_len * sizeof(seq_t));
        if (!tmp && new_len > 0)   /* allocation failure – abort */
            exit(1);

        /* old content is preserved by realloc; no extra copy needed */
        vals = tmp;

        /* compute terms from old length up to target */
        for (size_t i = len; i <= target; ++i)
        {
            /* coefficient from the numerator polynomial */
            seq_t p;
            if (i == 0U || i == 18U)
                p = 1;
            else if (i <= 17U)
                p = 2;
            else
                p = 0;

            /* sum of the last seventeen terms */
            seq_t sum = 0;
            size_t maxj = (i < 17U) ? i : 17U;
            for (size_t j = 1; j <= maxj; ++j)
                sum += vals[i - j];

            /* recurrence */
            seq_t term = p + (seq_t)46 * sum;
            if (i >= 18U)
                term -= (seq_t)1081 * vals[i - 18];

            vals[i] = term;
        }
        len = new_len;
    }

    return vals[(size_t)n];
}
