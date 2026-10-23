/*
 * A118299  Start with 24 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 1, a(1) = 24
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   ensure_capacity, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cap = 0;        /* allocated capacity */
    static idx_t computed = 0;    /* highest index already computed */

    if (n == 0) return 0;         /* below the offset – arbitrary */

    /* Ensure space for terms up to n */
    if ((size_t)n > cap)
        ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(seq_t));

    /* Return cached value if available */
    if (n <= computed)
        return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 24;
        } else {
            val = reverse_decimal(cache[(size_t)(i - 2)]) + 1;
        }
        cache[(size_t)(i - 1)] = val;
    }
    computed = n;
    return cache[(size_t)(n - 1)];
}
