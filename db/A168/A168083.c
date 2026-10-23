/*
 * A168083  Fibonacci 12-step numbers.
 * https://oeis.org/A168083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for 1 ≤ n ≤ 11; a(12) = 1; for n > 12, a(n) = a(n-1) +
 *            a(n-2) + … + a(n-12)
 * Range:     u8 n <= 20, u16 n <= 29, u32 n <= 45, u64 n <= 77, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[0] = a(1), cache[1] = a(2), … */
    static seq_t *cache = NULL;   /* array of computed terms                */
    static size_t cap = 0;        /* allocated array size                   */
    static idx_t len = 0;         /* number of terms currently stored       */

    /* Convert n to an index for array access. */
    size_t N = (size_t)n;
    if (N == 0) {
        /* Undefined for n == 0; return 0 for safety. */
        return 0;
    }

    /* Allocate more space if needed.  Handle the C++ realloc return type. */
    if (len < N) {
        if (N > cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap < N) {
                new_cap *= 2;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                abort();   /* allocation failed */
            }
            cache = tmp;
            cap = new_cap;
        }

        /* Compute terms up to N.  */
        for (size_t i = (size_t)len + 1; i <= N; ++i) {
            seq_t val;
            if (i == 12) {
                val = 1;                /* a(12) = 1 */
            } else {
                val = 0;
                /* Sum the previous up to 12 terms.  */
                for (size_t j = 1; j <= 12 && i > j; ++j) {
                    val += cache[i - j - 1];
                }
            }
            cache[i - 1] = val;
        }
        len = N;
    }

    return cache[N - 1];
}
