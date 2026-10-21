/*
 * A228157  Numbers k which are anagrams of k+9.
 * https://oeis.org/A228157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10, with a(1) = 12,
 *            a(2) = 23, a(3) = 34, a(4) = 45, a(5) = 56, a(6) = 67, a(7) =
 *            78, a(8) = 89, a(9) = 101, a(10) = 112
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) {
        return 0;               /* undefined for n < 1 */
    }

    /* enlarge cache if needed */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp;
#if defined(__cplusplus)
        tmp = reinterpret_cast<seq_t*>(realloc(cache,
                                                (size_t)new_size * sizeof(seq_t)));
#else
        tmp = (seq_t*)realloc(cache,
                              (size_t)new_size * sizeof(seq_t));
#endif
        if (!tmp) abort();      /* memory allocation failure */
        cache = tmp;

        /* fill missing entries (indices are 1‑based in the sequence) */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 10) {
                /* initial 10 terms */
                static const seq_t base[10] = {
                    12, 23, 34, 45, 56, 67, 78, 89, 101, 112
                };
                cache[i - 1] = base[i - 1];
            } else {
                /* recurrence: a(n) = a(n-1) + a(n-9) - a(n-10) */
                cache[i - 1] = cache[i - 2]      /* a(i-1) */
                            + cache[i - 10]     /* a(i-9) */
                            - cache[i - 11];    /* a(i-10) */
            }
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
