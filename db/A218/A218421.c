/*
 * A218421  Number of n X 3 binary arrays with every 0 a horizontal, vertical
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A218421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 16*a(n-2) + 28*a(n-3) + 17*a(n-4) + 7*a(n-5) -
 *            3*a(n-6) + 3*a(n-7) - 8*a(n-8) - a(n-10) + a(n-11) Initial
 *            values: a(1) = 5, a(2) = 49, a(3) = 357, a(4) = 2725, a(5) =
 *            20793, a(6) = 158429, a(7) = 1207525, a(8) = 9203377, a(9) =
 *            70144889, a(10)= 534620525, a(11)= 4074695269
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* 1‑based cache: cache[i] holds a(i) for i >= 1 */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* allocated slots (including index 0) */
    static size_t computed_max = 0;        /* largest n with cache entry computed */
    static int initialized = 0;           /* flag for initializing base terms */

    if (n == 0) return 0;                 /* behaviour below offset irrelevant */

    /* Ensure cache can hold indices up to n.  Allocate at least 12 slots
       (for indices 0..11) so that recurrence indices never underflow. */
    size_t needed = (size_t)n + 1;
    if (needed < 12) needed = 12;
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise the first 11 values once. */
    if (!initialized) {
        cache[1]  = 5U;
        cache[2]  = 49U;
        cache[3]  = 357U;
        cache[4]  = 2725U;
        cache[5]  = 20793U;
        cache[6]  = 158429U;
        cache[7]  = 1207525U;
        cache[8]  = 9203377U;
        cache[9]  = 70144889U;
        cache[10] = 534620525U;
        cache[11] = 4074695269U;
        computed_max = 11;
        initialized = 1;
    }

    /* If we already have the desired value, return it. */
    if ((size_t)n <= computed_max) return cache[(size_t)n];

    /* Compute any missing values up to n. */
    for (idx_t i = (idx_t)(computed_max + 1); i <= n; ++i) {
        size_t idx = (size_t)i;
        /* The recurrence requires indices i-1 .. i-11 (i-9 is not used). */
        seq_t val = 0;
        val += (seq_t) 5 * cache[idx - 1];
        val += (seq_t)16 * cache[idx - 2];
        val += (seq_t)28 * cache[idx - 3];
        val += (seq_t)17 * cache[idx - 4];
        val += (seq_t) 7 * cache[idx - 5];
        val -= (seq_t) 3 * cache[idx - 6];
        val += (seq_t) 3 * cache[idx - 7];
        val -= (seq_t) 8 * cache[idx - 8];
        /* cache[idx-9] is not needed */
        val -= (seq_t) 1 * cache[idx -10];
        val += (seq_t) 1 * cache[idx -11];
        cache[idx] = val;
    }
    computed_max = (size_t)n;
    return cache[(size_t)n];
}
