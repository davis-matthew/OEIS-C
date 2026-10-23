/*
 * A118529  Start with 1 and repeatedly reverse the digits and add 12 to get
 *          the next term.
 * https://oeis.org/A118529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = rev_decimal(a(n-1)) + 12, a(1)=1
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, reverse_decimal, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed terms      */
    static idx_t cache_len = 0;     /* number of stored terms       */

    if (n == 0) return (seq_t)-1;   /* undefined for n<1            */

    /* initialise first two terms */
    if (cache_len == 0) {
        cache = seq_alloc_new_array(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        cache_len = 2;
    }

    /* return stored value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge cache to hold at least n terms */
    idx_t old_len = cache_len;
    grow_seq_array(&cache, &cache_len, n);

    /* compute missing terms */
    for (idx_t i = old_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[i] = rev + (seq_t)12;
    }

    cache_len = n;
    return cache[n - 1];
}
