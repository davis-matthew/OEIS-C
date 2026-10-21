/*
 * A200183  Number of -n..n arrays x(0..4) of 5 elements with zero sum and no
 *          two consecutive declines, no adjacent equal elements, and no
 *          element more than one greater than the previous (random base
 *          sawtooth pattern).
 * https://oeis.org/A200183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6)-a(n-7)+a(n-9) for n>10
 *            Initial values: a(1) = 2, a(2) = 12, a(3) = 15, a(4) = 24, a(5)
 *            = 31, a(6) = 48, a(7) = 53, a(8) = 74, a(9) = 83, a(10) = 108
 * Range:     u8 n <= 16, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;        /* 1‑based cache: cache[i] holds a(i) */
static idx_t cached_n = 0;         /* largest n already computed */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard against callers with 0 */

    /* Grow cache if necessary */
    if (n > cached_n) {
        size_t new_size = (size_t)n + 1;   /* indices 0…n */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure: return 0 (should never happen in tests) */
            return 0;
        }
        /* Compute new values */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t val;
            switch (i) {
                case 1:  val = 2;   break;
                case 2:  val = 12;  break;
                case 3:  val = 15;  break;
                case 4:  val = 24;  break;
                case 5:  val = 31;  break;
                case 6:  val = 48;  break;
                case 7:  val = 53;  break;
                case 8:  val = 74;  break;
                case 9:  val = 83;  break;
                case 10: val = 108; break;
                default:
                    val = cache[i-2] + cache[i-3] + cache[i-4]
                          - cache[i-5] - cache[i-6] - cache[i-7]
                          + cache[i-9];
                    break;
            }
            cache[i] = val;
        }
        cached_n = n;
    }
    return cache[n];
}
