/*
 * A170398  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence A170398: number of reduced words of length n in a Coxeter
   group on 5 generators.  The generating function is
       G(t) = (t^44 + 2 t^43 + … + 2 t + 1) /
              (1 - 3 t - 3 t^2 - … - 3 t^43 + 6 t^44).
   From this we obtain:
       a(0) = 1
       a(n) = 2 + 3 · Σ_{k=0}^{n-1} a(k)          (1 ≤ n ≤ 43)
       a(44)= 1 + 3 · Σ_{k=0}^{43} a(k)
       a(n) = 3·Σ_{k=1}^{43} a(n-k) – 6·a(n-44)   (n ≥ 45)
   The terms for n ≤ 44 are pre‑computed, afterwards the recurrence
   is used.  Cached values are stored so that any order of calls is
   handled efficiently.  */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values a[0] … */
    static idx_t cache_size = 0;    /* number of values stored */
    static size_t cache_capacity = 0; /* allocated capacity */

    /* Initialise first 45 terms if necessary */
    if (cache_size < 45) {
        size_t init_cap = 64;                               /* safe start */
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache_capacity = init_cap;

        cache[0] = 1;               /* a(0) */
        seq_t prefix = cache[0];     /* Σ_{k=0}^{i-1} a(k) for current i */

        for (idx_t i = 1; i <= 44; ++i) {
            seq_t p = (i == 44) ? 1 : 2;   /* numerator coefficient at t^i */
            cache[i] = p + prefix * 3;     /* (1) or (2) above */
            prefix += cache[i];
        }
        cache_size = 45;              /* terms 0 … 44 are known */
    }

    /* Expand capacity if the caller requests an index beyond it */
    if ((idx_t)(n + 1) > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 8;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* Compute any missing values up to n via the linear recurrence. */
    while (cache_size <= n) {
        idx_t i = cache_size;
        seq_t sum = 0;                           /* Σ_{k=1}^{43} a(i-k) */
        for (idx_t j = 1; j <= 43; ++j)
            sum += cache[i - j];
        seq_t val = sum * 3 - cache[i - 44] * 6; /* recurrence for n ≥ 45 */
        cache[i] = val;
        ++cache_size;
    }

    return cache[n];
}
