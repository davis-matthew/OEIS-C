/*
 * A211255  Number of (n+1)X(n+1) -6..6 symmetric matrices with every 2X2
 *          subblock having sum zero and one or three distinct values.
 * https://oeis.org/A211255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+5*a(n-2)-36*a(n-3)+5*a(n-4)+123*a(n-5)
 *            -69*a(n-6)-205*a(n-7)+150*a(n-8)+176*a(n-9)
 *            -138*a(n-10)-74*a(n-11)+56*a(n-12)+12*a(n-13)-8*a(n-14) Initial
 *            values: a(1)=65, a(2)=143, a(3)=287, a(4)=567, a(5)=1109,
 *            a(6)=2145, a(7)=4163, a(8)=8041, a(9)=15609, a(10)=30271,
 *            a(11)=58955, a(12)...
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of allocated slots (0..n)   */
    static idx_t  cached_max = 0;    /* largest n that has been computed   */

    /* small table of the first 14 terms */
    static const seq_t init[14] = {
        (seq_t)65,   (seq_t)143,   (seq_t)287,   (seq_t)567,
        (seq_t)1109, (seq_t)2145,  (seq_t)4163,  (seq_t)8041,
        (seq_t)15609,(seq_t)30271, (seq_t)58955, (seq_t)114925,
        (seq_t)224757,(seq_t)440281
    };

    /* enlarge the cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;                /* allocation failed */
        cache = new_cache;
        cache_size = (size_t)(n + 1);
        cache[0] = 0;                            /* index 0 is unused */
    }

    /* compute terms up to n if we haven't done so yet */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 14) {
                /* initial values */
                cache[i] = init[i - 1];
            } else {
                /* linear recurrence of order 14 */
                cache[i] = (seq_t)4   * cache[i - 1]
                        + (seq_t)5   * cache[i - 2]
                        - (seq_t)36  * cache[i - 3]
                        + (seq_t)5   * cache[i - 4]
                        + (seq_t)123 * cache[i - 5]
                        - (seq_t)69  * cache[i - 6]
                        - (seq_t)205 * cache[i - 7]
                        + (seq_t)150 * cache[i - 8]
                        + (seq_t)176 * cache[i - 9]
                        - (seq_t)138 * cache[i -10]
                        - (seq_t)74  * cache[i -11]
                        + (seq_t)56  * cache[i -12]
                        + (seq_t)12  * cache[i -13]
                        - (seq_t)8   * cache[i -14];
            }
        }
        cached_max = n;
    }

    return cache[n];
}
