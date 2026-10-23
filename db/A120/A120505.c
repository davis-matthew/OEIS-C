/*
 * A120505  Generalized meta-Fibonacci sequence a(n) with parameters s=2 and
 *          k=3.
 * https://oeis.org/A120505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 1 (n = 1,2,3) * a(n) = n-2 (n = 4,5) * a(n) =
 *            a(n-2-a(n-1)) + a(n-3-a(n-2)) + a(n-4-a(n-3)) (n > 5) * * The
 *            function computes the sequence with memoisation. It stores
 *            previously * computed values in a static array so that calls are
 *            efficient even if the * indices are requested in arbitra...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, realloc */
#include <stddef.h>     /* size_t */

seq_t a(idx_t n)
{
    /* static cache: index 0 unused, cache[i] holds a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index cached */

    if (n == 0) return 0;         /* safety */

    /* expand cache if necessary */
    if (n > cache_len)
    {
        size_t new_len = (size_t)n + 1;           /* include index 0 */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;                /* allocation failed */
        cache = tmp;

        /* initialise new entries */
        for (idx_t k = cache_len + 1; k <= n; ++k)
        {
            if (k <= 3)
                cache[k] = 1;
            else if (k <= 5)
                cache[k] = (seq_t)(k - 2);
            else
            {
                /* indices for the recurrence */
                idx_t i1 = (idx_t)(k - 2) - (idx_t)cache[k - 1];
                idx_t i2 = (idx_t)(k - 3) - (idx_t)cache[k - 2];
                idx_t i3 = (idx_t)(k - 4) - (idx_t)cache[k - 3];

                /* fetch previously computed terms */
                seq_t v1 = cache[i1];
                seq_t v2 = cache[i2];
                seq_t v3 = cache[i3];

                cache[k] = v1 + v2 + v3;
            }
        }
        cache_len = n;
    }

    return cache[n];
}
