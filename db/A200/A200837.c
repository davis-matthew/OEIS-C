/*
 * A200837  Number of 0..7 arrays x(0..n+1) of n+2 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..7 arrays x(0..n+1) of n+2 elements without
 *            two consecutive increases or decreases Recurrence (offset 1):
 *            a(n) = 8*a(n-1) -12*a(n-2) +6*a(n-3) -10*a(n-4) +12*a(n-5)
 *            -11*a(n-6) +11*a(n-7) -6*a(n-8) +3*a(n-9) - a(n-10) Initial
 *            terms: a(1)=400, a(2)=2444, a(3)=15128, a(4)=93472, a...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For the offset 1, n must be >=1.  Return 0 for an illegal index. */
    if (n == 0)
        return 0;

    /* Static cache of computed values.  cache[0] is unused. */
    static seq_t *cache     = NULL;
    static idx_t  tier      = 0;      /* current cache size (capacity) */
    static idx_t  max_comp  = 0;      /* largest n that has been computed */

    /* 10 initial values (index 1..10).  Index 0 is unused. */
    static const seq_t init_vals[10] = {
        (seq_t)400,
        (seq_t)2444,
        (seq_t)15128,
        (seq_t)93472,
        (seq_t)577660,
        (seq_t)3570086,
        (seq_t)22063924,
        (seq_t)136360286,
        (seq_t)842739040,
        (seq_t)5208328180
    };

    /* Ensure the cache has space for indices up to n. */
    if ((idx_t)(n + 1) > tier) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        /* realloc keeps the old data if resizing to a larger block. */
        cache = new_cache;
        tier  = n + 1;
    }

    /* Initialise the first 10 entries if they are not already set. */
    if (max_comp < 10) {
        idx_t start = max_comp + 1;
        idx_t end   = (n < 10) ? n : 10;
        for (idx_t i = start; i <= end; ++i) {
            cache[i] = init_vals[i - 1];
        }
    }

    /* If the requested index is already computed, return it. */
    if (n <= max_comp)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= 10) {
            /* For indices 1..10, use the initial values. */
            cache[i] = init_vals[i - 1];
        } else {
            /* Recurrence of order 10.  All arithmetic is performed in seq_t. */
            seq_t res = 8   * cache[i - 1];
            res -= 12  * cache[i - 2];
            res += 6   * cache[i - 3];
            res -= 10  * cache[i - 4];
            res += 12  * cache[i - 5];
            res -= 11  * cache[i - 6];
            res += 11  * cache[i - 7];
            res -= 6   * cache[i - 8];
            res += 3   * cache[i - 9];
            res -=     cache[i - 10];
            cache[i] = res;
        }
    }

    max_comp = n;   /* Update the highest computed index. */
    return cache[n];
}
