/*
 * A079512  a(0)=1, a(1)=1; for n>1, a(n) = Sum_{i=0..n/2}
 *          binomial(n-i-1,i)*a(n-2i-1) + ((n+1) mod 2).
 * https://oeis.org/A079512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 1; For n > 1, a(n) = Σ_{i=0..⌊(n-1)/2⌋}
 *            binomial(n-i-1,i) * a(n-2i-1) + ((n+1) mod 2) // i.e. add 1 when
 *            n is even
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 37, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a cache to store already computed terms.
   The binomial coefficients are updated iteratively using the
   recurrence
        C_{i+1} = C_i * (n-2i-1)*(n-2i-2) / ((i+1)*(n-i-1)),
   which keeps all intermediate results integer and fits within the
   chosen integer type for the needed range. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t cache_len = 0;          /* number of terms stored (first unused index) */
    static idx_t cache_alloc = 0;        /* allocated slots */

    /* Initialise cache with the first two terms */
    if (cache == NULL) {
        cache_alloc = 4;                 /* small initial size */
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) exit(1);            /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Grow the cache if necessary */
    if (n >= cache_alloc) {
        idx_t new_alloc = n + 1;
        if (new_alloc < 64) new_alloc = 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* Terms 0 and 1 are already initialised */
        if (k < 2) continue;

        idx_t max_i = (k - 1) / 2;  /* floor((k-1)/2) */
        seq_t sum = 0;
        seq_t bin = (seq_t)1;       /* C(k-1,0) */

        for (idx_t i = 0; i <= max_i; ++i) {
            idx_t idx = k - 2 * i - 1;   /* a(n - 2i - 1) */
            sum += bin * cache[idx];

            if (i < max_i) {
                /* Update binomial coefficient to C(k-i-2, i+1) */
                seq_t num1 = (seq_t)(k - 2 * i - 1);
                seq_t num2 = (seq_t)(k - 2 * i - 2);
                seq_t den1 = (seq_t)(i + 1);
                seq_t den2 = (seq_t)(k - i - 1);
                /* Avoid intermediate overflow for 64‑bit types but still
                   perform the exact integer division. */
                seq_t numerator = bin * num1 * num2;
                seq_t denominator = den1 * den2;
                bin = numerator / denominator;
            }
        }

        /* Add the parity term: 1 if k is even, 0 otherwise */
        if ((k & 1ULL) == 0) sum += (seq_t)1;

        cache[k] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
