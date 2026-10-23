/*
 * A163225  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163225: Number of reduced words of length n in Coxeter group
 * 
 * Recurrence:
 *   a(0) = 1
 *   a(1) = 42
 *   a(2) = 1722
 *   a(3) = 70602
 *   a(4) = 2893821
 *   for n ≥ 5:
 *       a(n) = 40*a(n‑1) + 40*a(n‑2) + 40*a(n‑3) – 820*a(n‑4)
 *
 * This follows from the rational generating function
 *     (t^4 + 2t^3 + 2t^2 + 2t + 1)/(1 − 40t − 40t² − 40t³ + 820t⁴)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t sz     = 0;

    /* Grow the cache to at least n+1 elements */
    if (n >= sz) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {           /* allocation failed – return 0 */
            free(cache);
            cache = NULL;
            sz = 0;
            return 0;
        }
        cache = tmp;

        /* Fill missing entries */
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0)        cache[i] = 1;
            else if (i == 1)   cache[i] = 42;
            else if (i == 2)   cache[i] = 1722;
            else if (i == 3)   cache[i] = 70602;
            else if (i == 4)   cache[i] = 2893821;
            else {                 /* i >= 5 */
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];
                seq_t a4 = cache[i - 4];
                cache[i] = 40 * a1 + 40 * a2 + 40 * a3 - 820 * a4;
            }
        }
        sz = n + 1;
    }
    return cache[n];
}
