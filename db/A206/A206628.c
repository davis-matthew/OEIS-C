/*
 * A206628  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than four equal edges, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2) + 6*a(n-3) for n > 4, a(1) = 14, a(2)
 *            = 121, a(3) = 1085, a(4) = 9729
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation:
       indices 0..cap-1  (index 0 is unused and stays 0)    */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* capacity allocated in elements   */
    static idx_t high = 0;        /* highest index already computed   */

    /* Ensure space up to index n (and at least indices 1..4). */
    size_t need = (size_t)(n + 1);
    if (need < 5) need = 5;          /* we need 1..4 for the recurrence */

    if (cap < need) {
        size_t new_cap = cap == 0 ? need : cap;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE); /* out of memory */
        /* Zero the newly allocated area. */
        if (cap < new_cap)
            memset(cache + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise the first four terms (they are constant). */
    cache[1] = 14;
    cache[2] = 121;
    cache[3] = 1085;
    cache[4] = 9729;

    /* Ensure we have computed at least up to index 4. */
    if (high < 4) high = 4;

    /* Compute further terms if requested.  Recurrence starts at n = 5. */
    if (n > high) {
        for (idx_t i = high + 1; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)8
                     + cache[i-2] * (seq_t)8
                     + cache[i-3] * (seq_t)6;
        }
        high = n;
    }

    return cache[n];
}
