/*
 * A118632  Start with 1 and repeatedly reverse the digits and add 35 to get
 *          the next term.
 * https://oeis.org/A118632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118632: start with 1 and repeatedly reverse the digits and
   add 35 to get the next term.  a(1) = 1, a(n) = reverse(a(n-1)) + 35. */
#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* out of offset – value irrelevant */

    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t  count = 0;             /* number of terms stored (i.e. max n computed) */
    static size_t cap   = 0;             /* allocated capacity */

    /* grow storage if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                        /* allocation failed – return 0 */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= count)
        return cache[n - 1];

    /* Compute missing terms up to n */
    for (idx_t i = count + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 2];          /* a(i-1) is at cache[i-2] */
            seq_t rev  = reverse_digits(prev);
            cache[i - 1] = rev + (seq_t)35;
        }
    }
    count = n;
    return cache[n - 1];
}
