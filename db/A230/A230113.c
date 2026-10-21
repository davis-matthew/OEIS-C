/*
 * A230113  Digital root of summed Fibonacci and Lucas digital roots indexed
 *          by numbers not divisible by 2, 3 or 5.
 * https://oeis.org/A230113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = a(n-1) - a(n-16) + a(n-17) for n > 17. Base values
 *            (offset 1): a(1)=3, a(2)=4, a(3)=5, a(4)=6, a(5)=6, a(6)=5,
 *            a(7)=4, a(8)=3, a(9)=4, a(10)=6, a(11)=6, a(12)=5, a(13)=4,
 *            a(14)=3, a(15)=3, a(16)=5, a(17)=6
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */
#include <stddef.h>   /* size_t */

#ifdef __cplusplus
#define PTR_CAST(type, ptr) reinterpret_cast<type>(ptr)
#else
#define PTR_CAST(type, ptr) ((type)(ptr))
#endif

seq_t a(idx_t n) {
    /* Per-call static cache of computed terms; cache[0] = a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */

    if (n < 1) return 0;   /* undefined for n = 0, but harmless */

    /* Allocate or grow the cache if necessary */
    if (!cache || n > cache_len) {
        size_t req_len = (size_t)n;
        /* Ensure the cache can at least hold the first 17 terms */
        size_t new_len = req_len > 17 ? req_len : 17;

        seq_t *tmp = PTR_CAST(seq_t *, realloc(cache, new_len * sizeof(seq_t)));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* Initial values for a(1) .. a(17) */
        static const seq_t init[17] = {
            3, 4, 5, 6, 6, 5,
            4, 3, 4, 6, 6, 5,
            4, 3, 3, 5, 6
        };

        /* Copy missing base terms */
        for (size_t i = cache_len; i < new_len && i < 17; ++i)
            cache[i] = init[i];

        /* Compute new terms using the recurrence */
        for (size_t j = (cache_len < 17 ? 17 : cache_len); j < new_len; ++j) {
            seq_t val = cache[j - 1];       /* a(n-1)   */
            val -= cache[j - 16];           /* subtract a(n-16) */
            val += cache[j - 17];           /* add a(n-17) */
            cache[j] = val;
        }

        cache_len = new_len;
    }

    return cache[(size_t)(n - 1)];
}
