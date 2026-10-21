/*
 * A063328  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          55 ).
 * https://oeis.org/A063328
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) – a(n-10), n > 13 Initial values: a(0) =
 *            0, a(1) = 0, a(2) = -1, a(3) = 79, a(4) = 190, a(5) = 298, a(6)
 *            = 406, a(7) = 514, a(8) = 622, a(9) = 730, a(10) = 838, a(11) =
 *            946, a(12) = 1054, a(13) = 1166
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment */
extern seq_t a(idx_t n);  /* declaration to satisfy the compiler */

seq_t a(idx_t n)
{
    /* Initial sequence values for indices 0..13 */
    static const seq_t init_vals[14] = {0, 0,
        -1, 79, 190, 298, 406, 514,
        622, 730, 838, 946, 1054, 1166};

    /* Static cache for memoization */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static idx_t cache_cap  = 0;    /* allocated capacity (number of indices) */
    static idx_t cache_size = 0;    /* number of terms actually filled */

    /* Grow the cache to accommodate index n */
    if (n >= cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;          /* double until we have enough */

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)               /* allocation failed; terminate safely */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n >= cache_size)
    {
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0 || i == 1)
            {
                cache[i] = 0;
            }
            else if (i <= 13)
            {
                cache[i] = init_vals[i];
            }
            else
            {
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
        cache_size = n + 1;
    }

    /* For n < 2 the sequence is not defined; return 0 */
    if (n < 2)
        return 0;

    return cache[n];
}
