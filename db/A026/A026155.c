/*
 * A026155  T(n,n-4), where T is the array in A026148.
 * https://oeis.org/A026155
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 12*a(n-1) - 53*a(n-2) + 90*a(n-3) + 26*a(n-4)
 *            -256*a(n-5) + 160*a(n-6) + 224*a(n-7) -203*a(n-8)
 *            -84*a(n-9) + 77*a(n-10) + 14*a(n-11) -7*a(n-12)
 * Initial values: a(4)=1, a(5)=4, a(6)=16, a(7)=56, a(8)=189, a(9)=616,
 *                 a(10)=1967, a(11)=6182, a(12)=19205, a(13)=59124,
 *                 a(14)=180726, a(15)=549276
 *
 * This implementation caches computed terms and expands the cache as needed.
 * It can be used with any seq_t/idx_t pair supported by the build system.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const int init_vals[] = {
    1, 4, 16, 56, 189, 616,
    1967, 6182, 19205, 59124,
    180726, 549276
};
static const int coeffs[12] = {
    12, -53, 90, 26, -256, 160,
    224, -203, -84, 77, 14, -7
};

seq_t a(idx_t n)
{
    if (n < 4u) return 0;

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t max_comp = 0;
    static idx_t cap = 0;

    /* First allocation: capacity for indices 0..15 */
    if (!cache) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure: return 0 */
        memset(cache, 0, cap * sizeof(seq_t));

        /* Store initial values for n = 4 .. 15 */
        for (idx_t i = 4; i <= 15; ++i) {
            cache[i] = (seq_t)init_vals[i - 4];
        }
        max_comp = 15;
    }

    /* Ensure the cache can hold index n */
    if (n > max_comp) {
        if (n + 1 > cap) {
            idx_t new_cap = cap * 2;
            if (n + 1 > new_cap) new_cap = n + 1;
            seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            memcpy(tmp, cache, cap * sizeof(seq_t));
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
            free(cache);
            cache = tmp;
            cap = new_cap;
        }

        /* Compute terms from max_comp + 1 up to n */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            __int128 v = 0;
            for (int k = 0; k < 12; ++k) {
                idx_t idx = i - k - 1;
                int c = coeffs[k];
                v += (__int128)c * (__int128)cache[idx];
            }
            cache[i] = (seq_t)v;
        }
        max_comp = n;
    }

    return cache[n];
}
