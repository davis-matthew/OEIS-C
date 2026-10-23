/*
 * A168715  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 0 a(0) = 1 for 1 <= n <= 16 a(n) = 36 * Σ_{i=1}^{n}
 *            a(n-i) + 2 for n = 17 a(17) = 36 * Σ_{i=1}^{16} a(17-i) - 665
 *            for n >= 18 a(n) = 36 * Σ_{i=1}^{16} a(n-i) - 666 * a(n-17)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* <stddef.h> for size_t and NULL, <stdlib.h> for malloc/realloc/free */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* cached sequence values a[0..cached_up_to] */
    static idx_t cached_up_to = 0;   /* maximum index already computed */
    static idx_t capacity  = 0;      /* allocated array size */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) abort();     /* simple error handling */
        cache[0] = (seq_t)1;            /* a(0) */
        cached_up_to = 0;
    }

    /* expand cache if n exceeds current capacity */
    if (n >= capacity) {
        idx_t new_cap = capacity > 0 ? capacity : 1;
        while (new_cap <= n) new_cap <<= 1;    /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t m = cached_up_to + 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t limit = m < 16 ? m : 16;          /* number of terms to sum */

        for (idx_t i = 1; i <= limit; ++i)
            sum += cache[m - i];

        if (m <= 16) {
            cache[m] = (seq_t)36 * sum + (seq_t)2;
        } else if (m == 17) {
            /* a(17) = 36*Σ a(17-i) – 665   ; 665 = 666*a(0) – 1   */
            cache[m] = (seq_t)36 * sum - (seq_t)665;
        } else {
            cache[m] = (seq_t)36 * sum - (seq_t)666 * cache[m - 17];
        }
    }

    cached_up_to = n;
    return cache[n];
}
