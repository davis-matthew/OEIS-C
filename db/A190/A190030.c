/*
 * A190030  Number of (n+2) X 8 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 56, u64 n <= 128, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
 * for n > 14:
 *   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-6) - a(n-7)
 * Initial values (1 ≤ n ≤ 14):
 *   a(1)=880  a(2)=295  a(3)=430  a(4)=670
 *   a(5)=970  a(6)=1230 a(7)=1494 a(8)=1834
 *   a(9)=2338 a(10)=3142 a(11)=4226 a(12)=5674
 *   a(13)=7686 a(14)=10543
 */

#include <stdlib.h>
#include <stdint.h>

/* Assume seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 as a harmless value. */
    if (n == 0) return 0;

    static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
    static idx_t max_computed = 0;   /* largest n whose value is stored */

    /* Allocate or resize cache if necessary. */
    if (n > max_computed) {
        idx_t new_size = n;
        seq_t *new_cache = NULL;

        if (cache == NULL) {
            new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        } else {
            new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        }

        /* In the unlikely event realloc fails in subsequent calls,
         * keep the old cache and skip further computation. */
        if (new_cache == NULL) {
            return 0; /* graceful degradation */
        }

        /* Initial known values (cast to seq_t). */
        const uint64_t init_vals[14] = {
            880ULL, 295ULL, 430ULL, 670ULL,
            970ULL, 1230ULL, 1494ULL, 1834ULL,
            2338ULL, 3142ULL, 4226ULL, 5674ULL,
            7686ULL, 10543ULL
        };

        /* Fill the cache up to n. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 14) {
                /* zero‑based index i-1 */
                new_cache[i - 1] = (seq_t)init_vals[i - 1];
            } else {
                /* recurrence indices are zero‑based */
                idx_t k = i - 1;
                seq_t val = 2 * new_cache[k - 1]
                            - new_cache[k - 2]
                            + new_cache[k - 3]
                            - 2 * new_cache[k - 4]
                            + new_cache[k - 5]
                            + new_cache[k - 6]
                            - new_cache[k - 7];
                new_cache[k] = val;
            }
        }

        cache = new_cache;
        max_computed = n;
    }

    return cache[n - 1];
}
