/*
 * A170698  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 17 * 16^(n-1) for 1 ≤ n ≤ 49; a(50) = 17*16^49
 *            - 136; a(n) = 15 * Σ_{k=1}^{49} a(n-k) – 120 * a(n-50) for n ≥
 *            51
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* --- memoization cache ------------------------------------------- */
    static seq_t *cache = NULL;        /* array of previously computed terms   */
    static size_t cap   = 0;           /* allocated capacity                  */
    static idx_t  len   = 0;           /* number of terms actually stored     */

    /* --- grow cache if necessary ------------------------------------- */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL)
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* --- generate missing terms -------------------------------------- */
    if (len <= n) {
        for (idx_t i = len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            }
            else if (i <= 49) {               /* 1 … 49 : 17·16^(i‑1)          */
                seq_t pow = (seq_t)1;
                for (idx_t j = 0; j < i - 1; ++j)
                    pow = pow * (seq_t)16;
                val = (seq_t)17 * pow;
            }
            else if (i == 50) {              /* 50 : 17·16^49 – 136            */
                seq_t pow = (seq_t)1;
                for (idx_t j = 0; j < 49; ++j)
                    pow = pow * (seq_t)16;
                val = (seq_t)17 * pow - (seq_t)136;
            }
            else {                            /* n ≥ 51 : linear recurrence    */
                seq_t sum = (seq_t)0;
                for (idx_t k = 1; k <= 49; ++k)
                    sum += cache[i - k];
                val = (seq_t)15 * sum - (seq_t)120 * cache[i - 50];
            }
            cache[i] = val;
        }
        len = n + 1;
    }

    return cache[n];
}
