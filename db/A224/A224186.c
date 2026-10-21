/*
 * A224186  Number of n X 4 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224186
   Formula: a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
            + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
       for n > 9;
   Initial values:
     a(1)=46, a(2)=548, a(3)=3526, a(4)=15779, a(5)=55438,
     a(6)=163746, a(7)=424326, a(8)=992607, a(9)=2138488.
   The function memoises previously computed values and works for
   arbitrary‑precision seq_t or any unsigned integer type. */

#include <stdlib.h>

static const seq_t base_values[9] = {
    46,      548,     3526,     15779,    55438,
    163746,  424326,   992607,   2138488   /* a(1) … a(9) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) */
    static idx_t max_cached = 0;  /* highest index stored, 0 meaning empty */

    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    /* For n <= 9 we can answer directly from the constant table */
    if (n <= 9)
        return base_values[n - 1];

    /* Ensure that the cache can hold the requested index */
    if (max_cached < n) {
        size_t new_cap = (size_t)n + 1;           /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                             /* out of memory */
        cache = tmp;

        /* Initialise the cache with the first 9 values if not already done */
        if (max_cached < 9) {
            cache[0] = (seq_t)0;                  /* unused */
            for (idx_t i = 1; i <= 9; ++i)
                cache[i] = base_values[i - 1];
            max_cached = 9;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos = ((seq_t)9)  * cache[i - 1]
                  + ((seq_t)84) * cache[i - 3]
                  + ((seq_t)126) * cache[i - 5]
                  + ((seq_t)36) * cache[i - 7]
                  + cache[i - 9];

        seq_t neg = ((seq_t)36) * cache[i - 2]
                  + ((seq_t)126) * cache[i - 4]
                  + ((seq_t)84) * cache[i - 6]
                  + ((seq_t)9)  * cache[i - 8];

        cache[i] = pos - neg;
    }

    max_cached = n;
    return cache[n];
}
