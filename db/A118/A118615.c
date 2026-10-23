/*
 * A118615  Start with 1 and repeatedly reverse the digits and add 26 to get
 *          the next term.
 * https://oeis.org/A118615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) = reverse_decimal(a(n-1)) + 26 */
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    /* static cache for values already computed */
    static seq_t *cache = NULL;       /* array indexed by 1 … cap */
    static size_t cap = 0;            /* maximum index for which array space exists   */
    static size_t computed = 0;       /* highest index whose value is stored in cache */

    if (n < 1) return (seq_t)0;       /* undefined, but harmless */

    /* First allocation or ensure enough space for n */
    if (cache == NULL) {
        size_t need = n > 1 ? n : 1;          /* at least space for a(1) */
        seq_t *tmp = (seq_t *)realloc(NULL, (need + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cap = need;
    } else if ((idx_t)cap < n) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise first term if not done yet */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[i]   = rev + (seq_t)26;
    }
    computed = (size_t)n;

    return cache[n];
}
