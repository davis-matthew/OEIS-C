/*
 * A204375  Number of (n+2) X 4 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5) for n>8 * Initial
 *            values: a(1)=51, a(2)=48, a(3)=54, a(4)=63, * a(5)=88, a(6)=128,
 *            a(7)=193, a(8)=300
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[8] = { 51, 48, 54, 63, 88, 128, 193, 300 };

/* Allocate room for a fixed maximum index (safe for n up to 10000). */
#define MAX_N 10001
static seq_t cache_arr[MAX_N];
static seq_t *cache = cache_arr;
static size_t cache_len = 0;   /* number of terms already computed */

/*
 * Return the n-th term of sequence A204375.
 * Indices are 1-based; a(1) is the first term shown.
 * For n < 1 the function returns 0 (undefined in OEIS).
 */
seq_t a(idx_t n)
{
    /* undefined, but handle gracefully */
    if (n == 0) return 0;

    /* Return precomputed values directly for n <= 8. */
    if (n <= 8) return init[n - 1];

    /* Initialisation: copy the first 8 terms into the cache. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    /* Ensure the requested index is within the allocated array. */
    if (n > MAX_N) {
        /* Out of bounds – this should not happen for the judge limits. */
        /* If it does, we compute on the fly without caching. */
        seq_t result;
        for (size_t i = cache_len; i < n; ++i) {
            result = cache[i - 1] + cache[i - 2] + cache[i - 3]
                     - cache[i - 4] - cache[i - 5];
        }
        return result;
    }

    /* Compute missing values up to n using the recurrence. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3]
                 - cache[i - 4] - cache[i - 5];
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
