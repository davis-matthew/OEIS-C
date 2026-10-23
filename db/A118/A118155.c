/*
 * A118155  Start with 1 and repeatedly reverse the digits and add 59 to get
 *          the next term.
 * https://oeis.org/A118155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1) = 1, a(n) = rev_decimal(a(n-1)) + 59 for n > 1
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

#ifdef __cplusplus
#include <vector>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdint.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* below offset: harmless */

#ifdef __cplusplus
    /* C++ build: use std::vector for arbitrary‑precision types */
    static std::vector<seq_t> cache;
    if (cache.size() < (size_t)n) {
        cache.reserve(n);
        for (idx_t i = cache.size(); i < n; ++i) {
            if (i == 0) {
                cache.push_back((seq_t)1);
            } else {
                seq_t rev = reverse_decimal(cache[i-1]);
                cache.push_back(rev + (seq_t)59);
            }
        }
    }
    return cache[n-1];
#else
    /* C build: use malloc/realloc for native integer types */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;   /* number of terms stored */

    if (len < n) {
        size_t new_cap = (len == 0 ? 1 : len * 2);
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure, return 0 as fallback */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
        for (idx_t i = len; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else {
                seq_t rev = reverse_decimal(cache[i-1]);
                cache[i] = rev + (seq_t)59;
            }
        }
        len = n;
    }
    return cache[n-1];
#endif
}
