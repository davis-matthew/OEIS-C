/*
 * A067339  Divide the natural numbers in sets of consecutive numbers,
 *          starting with {1,2}, each set with number of elements equal to the
 *          sum of elements of the preceding set. The final element of the
 *          n-th set gives a(n).
 * https://oeis.org/A067339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(a(n-1)+1)/2 + 2 (a(1) = 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static inline seq_t mul_seq(seq_t a, seq_t b) {
#ifdef __cplusplus
    /* C++ build – seq_t is an arbitrary‑precision class */
    return a * b;
#else
    /* C build – use 128‑bit intermediate if available to avoid overflow */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)prod;
#else
    return a * b;
#endif
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* below offset – unused */

    static seq_t *cache = NULL;
    static idx_t len  = 0;      /* number of stored terms   */
    static idx_t cap  = 0;      /* allocated capacity       */

    /* initialise with a(1) */
    if (len == 0) {
        if (cap == 0) {
            cap = 4;
            cache = mem_resize(seq_t, cache, cap);
        }
        cache[0] = (seq_t)2;    /* a(1) */
        len = 1;
    }

    if (n > len) {
        if (n > cap) {
            idx_t new_cap = cap ? cap * 2 : 4;
            if (new_cap < n) new_cap = n;
            cache = mem_resize(seq_t, cache, new_cap);
            cap = new_cap;
        }
        for (idx_t i = len + 1; i <= n; ++i) {
            idx_t idx = i - 1;               /* array index for a(i) */
            seq_t prev = cache[idx - 1];     /* a(i-1) */

            seq_t prod = mul_seq(prev, prev + (seq_t)1);
            seq_t val = prod / 2 + (seq_t)2;
            cache[idx] = val;
        }
        len = n;
    }
    return cache[n - 1];
}
