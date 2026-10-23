/*
 * A170418  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170418 - number of reduced words of length n in a Coxeter group
   with 25 generators and relations (S_i)^2 = (S_i S_j)^4 = I.
   Recurrence (derived from the rational generating function):
      a(0) = 1;
      for 1 ≤ n ≤ 43:   a(n) = 2 + 23·∑_{i=1}^{n} a(n‑i);
      a(44) = 1 + 23·∑_{i=1}^{43} a(44‑i) – 276·a(0);
      for n > 44:       a(n) = 23·∑_{i=1}^{43} a(n‑i) – 276·a(n‑44);
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size  = 0;   /* number of computed terms */
    static idx_t cache_cap   = 0;   /* current allocation size */

    /* Ensure sufficient storage */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t rhs = 0;
        if (k == 0 || k == 44)
            rhs = 1;
        else if (k <= 43)
            rhs = 2;          /* RHS is 2 for 1 ≤ k ≤ 43 */

        seq_t sum = 0;
        idx_t m = (k < 43) ? k : 43;          /* number of terms to sum */
        for (idx_t i = 1; i <= m; ++i)
            sum += cache[k - i] * 23;

        if (k >= 44)
            sum -= cache[k - 44] * 276;

        cache[k] = rhs + sum;
    }

    cache_size = n + 1;
    return cache[n];
}
