/*
 * A200456  Number of -n..n arrays x(0..3) of 4 elements with zero sum and
 *          nonzero first and second differences.
 * https://oeis.org/A200456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A200456: Number of –n..n arrays of 4 elements with zero
 * sum and nonzero first and second differences.
 * Empirical recurrence:
 *   a(n) = a(n-1) +2*a(n-3) -2*a(n-4) +a(n-5) -2*a(n-6) +a(n-7)
 *          -2*a(n-8) +2*a(n-9) +a(n-11) -a(n-12)
 * Initial values (1‑based):
 *   a(1)=4, a(2)=44, a(3)=142, a(4)=342, a(5)=678, a(6)=1148,
 *   a(7)=1832, a(8)=2744, a(9)=3874, a(10)=5334, a(11)=7114, a(12)=9192
 * The function memoises computed terms up to a modest maximum (100 000)
 * so that any call with n ≤ 100 000 runs in O(n) time and O(1)
 * additional memory beyond the static cache.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* static cache: enough for the required problem size */
    #define CACHE_MAX 100000
    static seq_t cache[CACHE_MAX + 1];
    static idx_t cache_size = 0;  /* highest n that has been computed */

    /* initialise the first 12 terms once */
    if (cache_size == 0U) {
        cache[1]  = (seq_t)  4;
        cache[2]  = (seq_t) 44;
        cache[3]  = (seq_t)142;
        cache[4]  = (seq_t)342;
        cache[5]  = (seq_t)678;
        cache[6]  = (seq_t)1148;
        cache[7]  = (seq_t)1832;
        cache[8]  = (seq_t)2744;
        cache[9]  = (seq_t)3874;
        cache[10] = (seq_t)5334;
        cache[11] = (seq_t)7114;
        cache[12] = (seq_t)9192;
        cache_size = 12U;
    }

    /* If the requested n is already known, just return it. */
    if (n <= cache_size) {
        return cache[n];
    }

    /* Guard against exceeding the static cache size. */
    if (n > CACHE_MAX) {
        /* In this implementation we simply clamp to CACHE_MAX.
         * For larger n a more generous cache or dynamic allocation
         * could be employed. */
        n = CACHE_MAX;
    }

    /* Compute any missing terms iteratively using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Positive coefficient part */
        seq_t pos = cache[i - 1];
        pos += (seq_t)2 * cache[i - 3];
        pos += cache[i - 5];
        pos += cache[i - 7];
        pos += (seq_t)2 * cache[i - 9];
        pos += cache[i - 11];

        /* Negative coefficient part */
        seq_t neg = (seq_t)2 * cache[i - 4];
        neg += (seq_t)2 * cache[i - 6];
        neg += (seq_t)2 * cache[i - 8];
        neg += cache[i - 12];

        /* Combine to get a(i) */
        cache[i] = pos - neg;
    }

    cache_size = n;
    return cache[n];
}
