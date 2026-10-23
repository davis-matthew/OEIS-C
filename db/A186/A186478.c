/*
 * A186478  Number of (n+1)X6 0..3 arrays with every 2X2 subblock commuting
 *          with each of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A186478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-4)-3*a(n-5) for n>16
 *            a(1) = 4895, a(2) = 3442, a(3) = 20224, a(4) = 25633, a(5) =
 *            107353, a(6) = 108025, a(7) = 433513, a(8) = 479785, a(9) =
 *            1793817, a(10) = 2226665, a(11) = 7478693, a(12) = 10225073,
 *            a(13) = 31371137, a(14) = 46298817, a(15) = 132181633,...
 * Range:     u8 none, u16 n <= 4, u32 n <= 20, u64 n <= 50, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap     = 0;          /* allocated size (indices 0..cap-1) */
    static idx_t computed_max = 0;      /* largest index computed so far */

    static const seq_t base_vals[17] = {
        0,      /* unused index 0 */
        (seq_t)4895,   (seq_t)3442,   (seq_t)20224,
        (seq_t)25633,  (seq_t)107353, (seq_t)108025,
        (seq_t)433513, (seq_t)479785, (seq_t)1793817,
        (seq_t)2226665,(seq_t)7478693,(seq_t)10225073,
        (seq_t)31371137,(seq_t)46298817,(seq_t)132181633,
        (seq_t)207648469
    };

    if (n <= 16)
        return base_vals[(size_t)n];

    /* Allocate / grow the cache to accommodate the requested index. */
    if (cache == NULL) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cap = new_cap;
        for (size_t i = 1; i <= 16; ++i)
            cache[i] = base_vals[i];
        computed_max = 16;
    } else if (cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cap = new_cap;
        /* base values are already filled; no further action needed. */
    }

    /* Compute any missing values up to n, using the recurrence. */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            cache[(size_t)i] =
                2 * cache[(size_t)(i - 1)] +
                2 * cache[(size_t)(i - 2)] -
                6 * cache[(size_t)(i - 3)] +
                6 * cache[(size_t)(i - 4)] -
                3 * cache[(size_t)(i - 5)];
        }
        computed_max = n;
    }

    return cache[(size_t)n];
}
