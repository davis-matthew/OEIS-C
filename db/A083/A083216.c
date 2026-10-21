/*
 * A083216  Fibonacci-like sequence of composite numbers with a(0) =
 *          20615674205555510, a(1) = 3794765361567513.
 * https://oeis.org/A083216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2), a(0) = 20615674205555510, a(1) =
 *            3794765361567513
 * Range:     u8 none, u16 none, u32 none, u64 n <= 16, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache up to n = 10000 (problem guarantees n ≤ ~10000) */
static seq_t cache[10001];
static size_t cache_len = 0;     /* number of initialized entries */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* base cases: return constants directly */
    if (n == 0)
        return (seq_t)20615674205555510ULL;
    if (n == 1)
        return (seq_t)3794765361567513ULL;

    /* initialize cache[0] and cache[1] once */
    if (cache_len == 0) {
        cache[0] = (seq_t)20615674205555510ULL;
        cache[1] = (seq_t)3794765361567513ULL;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        /* safety: guard against accidental overflow beyond array size */
        if ((size_t)n + 1 > 10001) {
            /* For the intended ranges (n ≤ 10000) this should never happen. */
            return (seq_t)0;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
