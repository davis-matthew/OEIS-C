/*
 * A237301  Number of (n+1)X(1+1) 0..2 arrays with the upper median of every
 *          2X2 subblock equal.
 * https://oeis.org/A237301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -143*a(n-3) +79*a(n-4) +796*a(n-5) -292*a(n-6)
 *            -1760*a(n-7) +256*a(n-8) +1280*a(n-9) with a(1)=81, a(2)=435,
 *            a(3)=2174, a(4)=11545, a(5)=61055, a(6)=326262, a(7)=1745153,
 *            a(8)=9357963, a(9)=50227854
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms (0‑based index for a(1) …) */
static seq_t *seq_cache = NULL;
static size_t seq_cached = 0;

static const seq_t init_vals[9] = {
    (seq_t)81,
    (seq_t)435,
    (seq_t)2174,
    (seq_t)11545,
    (seq_t)61055,
    (seq_t)326262,
    (seq_t)1745153,
    (seq_t)9357963,
    (seq_t)50227854
};

seq_t a(idx_t n)
{
    /* n is 1‑based.  Undefined for n=0 – return 0. */
    if (n == 0) return (seq_t)0;

    size_t needed = (size_t)n;

    /* Allocate or enlarge the cache as needed */
    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (!seq_cache) return (seq_t)0;
    } else if (needed > seq_cached) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * needed);
        if (!tmp) return (seq_t)0;
        seq_cache = tmp;
    }

    /* Compute any missing terms */
    for (size_t i = seq_cached; i < needed; ++i) {
        if (i < 9) {
            seq_cache[i] = init_vals[i];
        } else {
            /* Recurrence */
            seq_t term = (seq_t)9      * seq_cache[i-1]
                       + (seq_t)79     * seq_cache[i-4]
                       + (seq_t)796    * seq_cache[i-5]
                       + (seq_t)256    * seq_cache[i-8]
                       + (seq_t)1280   * seq_cache[i-9]
                       - (seq_t)143    * seq_cache[i-3]
                       - (seq_t)292    * seq_cache[i-6]
                       - (seq_t)1760   * seq_cache[i-7];
            seq_cache[i] = term;
        }
    }

    seq_cached = needed;
    return seq_cache[n-1];
}
