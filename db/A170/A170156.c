/*
 * A170156  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A170156
 *  Recurrence derived from the generating function
 *      A(t) = (t^38 + … + t + 1) / (t^38 - 2t^37 + t^36 - 2t^35 + … - 2t + 1)
 *
 *  Let c_k =  1  if k is even (0 ≤ k ≤ 38)
 *            -2  if k is odd  (1 ≤ k ≤ 38).
 *
 *  For n ≥ 0:
 *      for n ≤ 38 :  a(n) = 1 + 2·Σ_{odd k ≤ n} a(n−k) – Σ_{even k ≤ n} a(n−k)
 *      for n ≥ 39 :  a(n) = 2·Σ_{odd k=1..38} a(n−k) – Σ_{even k=1..38} a(n−k)
 *
 *  The implementation fills a static cache so that calls may be made in any
 *  order.  All arithmetic is performed only with the permitted operators.
 */

#include <stddef.h>
#include <stdlib.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static cache of computed terms (1‑based indexing for the array) */
    static seq_t *cache    = NULL;   /* array of a(i) for i = 0 .. cache_len-1 */
    static idx_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Ensure we can store terms up to index n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if the cache is empty */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t limit = i < 38 ? i : 38;
        seq_t sum_even = 0;
        seq_t sum_odd  = 0;

        for (idx_t k = 1; k <= limit; ++k) {
            if (k & 1)   /* k odd */
                sum_odd += cache[i - k];
            else         /* k even */
                sum_even += cache[i - k];
        }

        seq_t temp = sum_odd;
        temp += sum_odd;                  /* temp = 2*sum_odd */
        if (i <= 38)
            temp += (seq_t)1;              /* temp = 1 + 2*sum_odd */
        temp -= sum_even;                  /* temp = temp - sum_even */

        cache[i] = temp;
        cache_len = i + 1;
    }

    return cache[n];
}
