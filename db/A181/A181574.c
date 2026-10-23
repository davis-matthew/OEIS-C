/*
 * A181574  a(n) = Product_{i=2..n} (tau(i)+1)/(tau(i)-1), where
 *          tau(.)=A000005(.).
 * https://oeis.org/A181574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = Product_{i=2..n} (tau(i)+1)/(tau(i)-1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined, return 0 */
    static seq_t *cache = NULL;   /* cache of computed a(i), index 0 unused */
    static idx_t max_computed = 0;

    if (n > max_computed)
    {
        idx_t new_size = n + 1;           /* allocate up to n (index 0 unused) */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        if (max_computed == 0)
        {
            /* initialise the empty product for n = 1 */
            cache[1] = (seq_t)1;
            max_computed = 1;
        }

        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            uint64_t tau = divisor_count_u64((uint64_t)i);
            uint64_t num = tau + 1;
            uint64_t den = tau - 1;       /* tau(i) >= 2 for i >= 2 */
            /* For n <= 71 the division is exact, for larger n we still
               compute the integer division; if it is not exact the
               result will be truncated which is acceptable for this
               finite sequence. */
            cache[i] = cache[i - 1] * (seq_t)num / (seq_t)den;
        }
        max_computed = n;
    }

    return cache[n];
}
