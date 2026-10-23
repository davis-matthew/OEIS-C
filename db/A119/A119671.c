/*
 * A119671  a(0)=1. a(2^m +k) = a(m) + k, where 0 <= k <= 2^m -1, m >= 0.
 * https://oeis.org/A119671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(2^m + k) = a(m) + k, where 0 <= k < 2^m and m >= 0
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Small helper: floor of log2 for unsigned 64-bit indices. */
static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    /* Guard against shifting by 64 when x is 0xFFFFFFFFFFFFFFFF. */
    while (r < 63 && ((idx_t)1 << (r + 1)) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* Base case: a(0) = 1 */

    static seq_t *cache = NULL;         /* memoization cache */
    static size_t cap = 0;              /* current capacity of cache */

    /* Expand cache if we need to store up to n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                /* allocation failure: unlikely for n <= 10000 */
            return (seq_t)0;

        /* Initialise new entries, using already computed smaller indices. */
        for (size_t i = cap; i < new_cap; ++i) {
            idx_t ii = (idx_t)i;
            if (ii == 0) {
                tmp[i] = (seq_t)1;
            } else {
                idx_t m = floor_log2_idx(ii);            /* highest power of two ≤ ii */
                idx_t k = ii - ((idx_t)1 << m);
                tmp[i] = tmp[m] + (seq_t)k;              /* a(m) + k */
            }
        }

        cache = tmp;
        cap   = new_cap;
    }

    return cache[(size_t)n];
}
