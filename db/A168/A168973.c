/*
 * A168973  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the given generating function:
   a(0) = 1
   for 1 ≤ n ≤ 22:      a(n) = 2 + 6 * Σ_{k=0}^{n-1} a(k)
   for n = 23:          a(23) = 6 * Σ_{k=1}^{22} a(23-k) - 21 * a(0) + 1
   for n > 23:          a(n) = 6 * Σ_{k=1}^{22} a(n-k) - 21 * a(n-23)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored a(i) values          */
    static size_t cap   = 0;          /* current allocated capacity */
    static size_t size  = 0;          /* largest index computed +1   */

    /* grow storage if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap < (size_t)(n + 1))
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i <= 22) {           /* 1 ≤ i ≤ 22 */
            seq_t sum = 0;
            for (size_t j = 0; j < i; ++j)
                sum += cache[j];
            val = 2 + 6 * sum;
        } else {
            seq_t sum = 0;
            for (size_t k = 1; k <= 22; ++k)
                sum += cache[i - k];
            if (i == 23)
                val = 6 * sum - 21 * cache[0] + 1;
            else
                val = 6 * sum - 21 * cache[i - 23];
        }
        cache[i] = val;
    }
    size = (size_t)n + 1;
    return cache[(size_t)n];
}
