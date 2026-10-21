/*
 * A236120  Number of (n+1) X (1+1) 0..3 arrays colored with the sets of
 *          distinct values in every 2 X 2 subblock.
 * https://oeis.org/A236120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 59*a(n-2) + 164*a(n-3) + 264*a(n-4) +
 *            96*a(n-5) * - 144*a(n-6) for n ≥ 7 * Initial values: * a(1) =
 *            256, a(2) = 3096, a(3) = 37584, * a(4) = 455808, a(5) = 5529360,
 *            a(6) = 67071552
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[7] = {
    0,          /* a(0) unused */
    256,        /* a(1) */
    3096,       /* a(2) */
    37584,      /* a(3) */
    455808,     /* a(4) */
    5529360,    /* a(5) */
    67071552    /* a(6) */
};

seq_t a(idx_t n)
{
    /* Static cache that persists across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;       /* allocated size (in elements) */
    static idx_t computed_max = 0;     /* highest n computed so far */

    if (n <= 6)
        return base_vals[n];

    /* Ensure cache is large enough for indices up to n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 to avoid crash (input range guarantees success) */
            return 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize base values if not already done */
    if (computed_max < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = base_vals[i];
        computed_max = 6;
    }

    /* Compute values up to n, continuing where we left off */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)6   * cache[i-1];
        val += (seq_t)59  * cache[i-2];
        val += (seq_t)164 * cache[i-3];
        val += (seq_t)264 * cache[i-4];
        val += (seq_t)96  * cache[i-5];
        val -= (seq_t)144 * cache[i-6];
        cache[i] = val;
    }
    computed_max = n;

    return cache[n];
}
