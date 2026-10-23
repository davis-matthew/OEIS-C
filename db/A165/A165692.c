/*
 * A165692  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(0)=1, and for n>=0 a(n) =
 *            39*(a(n-1)+...+a(n-8)) - 780*a(n-9) + c_n where c_n = 1 for n=0
 *            or n=9, 2 for 1<=n<=8, 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t len = 0;          /* number of terms currently stored (cache[0..len-1]) */

    /* enlarge storage if necessary – always keep at least space for 10 terms */
    size_t need = (size_t)(n + 1);
    if (need > capacity || capacity < 10) {
        if (capacity < 10) capacity = 10;
        if (need > capacity) capacity = need;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* coefficient from the numerator of the generating function */
        seq_t coef = 0;
        if (i == 0)          coef = (seq_t)1;
        else if (i <= 8)     coef = (seq_t)2;
        else if (i == 9)     coef = (seq_t)1;

        /* sum of the 8 preceding terms */
        seq_t sumPrev = 0;
        for (idx_t k = 1; k <= 8; ++k) {
            if (i > k - 1) /* i-k >= 0 */
                sumPrev += cache[i - k];
        }
        sumPrev *= (seq_t)39;

        /* subtract 780*a(i-9) if it exists */
        seq_t term9 = 0;
        if (i >= 9)
            term9 = (seq_t)780 * cache[i - 9];

        cache[i] = sumPrev - term9 + coef;
    }

    len = n + 1;
    return cache[n];
}
