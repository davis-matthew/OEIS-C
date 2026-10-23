/*
 * A142471  a(0) = a(1) = 0; thereafter a(n) = a(n-1)*a(n-2) + 2.
 * https://oeis.org/A142471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 0; for n >= 2, a(n) = a(n-1)*a(n-2) + 2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    size_t needed = (size_t)(n + 1);
    if (needed > capacity) {
        size_t old_cap = capacity;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[needed];
        for (idx_t i = 0; i < old_cap; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = realloc(cache, needed * sizeof(seq_t));
#endif
        if (!tmp) exit(1);  /* allocation failure */
        if (old_cap == 0) {
            tmp[0] = 0;
            if (needed > 1) tmp[1] = 0;
        }
        idx_t start = old_cap;
        if (old_cap <= 1) start = 2;
        for (idx_t k = start; k <= n; ++k) {
            tmp[k] = tmp[k-1] * tmp[k-2] + 2;
        }
        cache = tmp;
        capacity = (idx_t)needed;
    }
    return cache[n];
}
