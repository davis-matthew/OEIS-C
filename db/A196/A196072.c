/*
 * A196072  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,3,2,1,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + 2*a(n-5) Initial values: a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=0, a(5)=6 Recurrence valid for n≥6
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 56, u64 n <= 111,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of already computed values, 0..max_cached  */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* size of cache  */
    static idx_t max_computed = 0; /* largest index that has a valid entry */

    /* a(0) is undefined in OEIS, return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Grow cache if necessary */
    if (cap <= n) {
        idx_t new_cap = n + 1;          /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Base values (indices start at 1) */
    static const seq_t base_vals[6] = {0, 0, 1, 2, 0, 6};

    /* fill base values for any newly allocated slots up to index 5 */
    if (max_computed < 5) {
        idx_t first_new = max_computed + 1;
        idx_t last_needed = cap > 5 ? 5 : cap - 1;
        for (idx_t i = first_new; i <= last_needed; ++i)
            cache[i] = base_vals[i];
        if (last_needed > max_computed)
            max_computed = last_needed;
    }

    /* If the requested index is already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute remaining values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 2] + cache[i - 3] + ((seq_t)2 * cache[i - 5]);
        cache[i] = val;
    }
    max_computed = n;

    return cache[n];
}
