/*
 * A025260  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 2, -2, 1, 2.
 * https://oeis.org/A025260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n ≥ 4,
 *            with initial values a(1)=2, a(2)=-2, a(3)=1, a(4)=2
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache (1‑based indices in the array) */
    static seq_t *seq = NULL;
    static idx_t cache_n = 0;   /* number of terms stored */

    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1 */

    /* initialize the first four terms */
    if (cache_n < 4)
    {
        idx_t init_cap = 4;
#ifdef __cplusplus
        seq = new seq_t[init_cap];
#else
        seq = (seq_t*)malloc(sizeof(seq_t) * (size_t)init_cap);
#endif
        seq[0] = (seq_t)2;   /* a(1) */
        seq[1] = (seq_t)-2;  /* a(2) */
        seq[2] = (seq_t)1;   /* a(3) */
        seq[3] = (seq_t)2;   /* a(4) */
        cache_n = init_cap;
    }

    /* return cached value if it already exists */
    if (n <= cache_n)
        return seq[n-1];

    /* grow the cache to hold terms up to n */
#ifdef __cplusplus
    seq_t *new_seq = new seq_t[n];
    for (idx_t i = 0; i < cache_n; ++i)
        new_seq[i] = seq[i];
    delete[] seq;
    seq = new_seq;
#else
    seq = (seq_t*)realloc(seq, sizeof(seq_t) * (size_t)n);
#endif

    /* compute the missing terms */
    for (idx_t j = cache_n + 1; j <= n; ++j)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= j-3; ++k)
        {
            sum += seq[k-1] * seq[j-1-k];
        }
        seq[j-1] = sum;
    }

    cache_n = n;
    return seq[n-1];
}
