/*
 * A169260  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=7*6^(n-1) for 1≤n<29,
 *            a(29)=42986595501253708480491, for n≥30: a(n)=
 *            (a(n-1)+a(n-2)+…+a(n-29))/3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size */
    static size_t max_n = 0;        /* highest index computed */
    static seq_t sum_last29 = (seq_t)0; /* sum of last 29 values (valid after 30) */
    static int sum_initialized = 0;     /* flag for sum_last29 */

    /* initial allocation */
    if (cache == NULL) {
        cap = (n < 30 ? n + 1 : 30);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* guarantee allocation success */
        if (cache == NULL) return (seq_t)0;  /* fallback: should never happen */
        /* base values */
        cache[0] = (seq_t)1;
        max_n = 0;
    }

    /* grow if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* assume realloc succeeds */
        cap = new_cap;
    }

    /* compute up to n if not already done */
    if ((size_t)n > max_n) {
        for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)7;
            } else if (i >= 2 && i <= 28) {
                cache[i] = cache[i - 1] * (seq_t)6;
            } else if (i == 29) {
                cache[i] = seq_from_str("42986595501253708480491");
            } else { /* i >= 30 */
                if (!sum_initialized) {
                    sum_last29 = (seq_t)0;
                    for (size_t j = 1; j <= 29; ++j) sum_last29 += cache[j];
                    sum_initialized = 1;
                }
                cache[i] = sum_last29 / (seq_t)3;
                /* update sliding sum */
                sum_last29 += cache[i];
                sum_last29 -= cache[i - 29];
            }
        }
        max_n = (size_t)n;
    }

    return cache[(size_t)n];
}
