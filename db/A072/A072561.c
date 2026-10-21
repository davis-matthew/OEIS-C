/*
 * A072561  Denominators of w(n) equals 3 where w(1)=w(2)=w(3)=1,
 *          w(n)=(w(n-1)*w(n-2)+(w(n-1)+w(n-2))/3) / w(n-3).
 * https://oeis.org/A072561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-10) - a(n-11) with initial values: a(1)=1,
 *            a(2)=3, a(3)=4, a(4)=6, a(5)=7, a(6)=9, a(7)=10, a(8)=12,
 *            a(9)=13, a(10)=15, a(11)=19
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* static cache for up to 20000 terms (covers all required tests) */
static seq_t seq_cache[20001];
static idx_t seq_cached_max = 0;

extern seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* strict offset 1 */

    /* If n exceeds the static buffer, fallback to dynamic handling
       (unlikely in the given tests). */
    if (n > 20000) {
#ifdef __cplusplus
        static std::vector<seq_t> vec;
        if (vec.size() < (size_t)n) {
            vec.resize(n);
            static const seq_t init[11] =
                {1,3,4,6,7,9,10,12,13,15,19};
            /* initialise first 11 terms */
            for (size_t i = 0; i < 11 && i < vec.size(); ++i)
                vec[i] = init[i];
            /* compute remaining terms using recurrence */
            for (size_t k = 12; k <= (size_t)n; ++k)
                vec[k-1] = vec[k-2] + vec[k-11] - vec[k-12];
        }
        return vec[n-1];
#else
        static seq_t *dyn = NULL;
        static size_t dyn_cap = 0;
        if (!dyn || dyn_cap < (size_t)n) {
            size_t new_cap = dyn_cap ? dyn_cap : 16;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(dyn, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            dyn = tmp;
            dyn_cap = new_cap;
            static const seq_t init[11] =
                {1,3,4,6,7,9,10,12,13,15,19};
            for (size_t i = 0; i < 11 && i < dyn_cap; ++i)
                dyn[i] = init[i];
            idx_t start = (dyn_cap < 12) ? 12 : dyn_cap + 1;
            for (idx_t k = start; k <= n; ++k)
                dyn[k-1] = dyn[k-2] + dyn[k-11] - dyn[k-12];
            seq_cached_max = n;
        }
        return dyn[n-1];
#endif
    }

    /* Use the static cache for n <= 20000 */
    if (seq_cached_max < n) {
        static const seq_t init[11] =
            {1,3,4,6,7,9,10,12,13,15,19};
        for (idx_t k = seq_cached_max + 1; k <= n; ++k) {
            if (k <= 11)
                seq_cache[k-1] = init[k-1];
            else
                seq_cache[k-1] = seq_cache[k-2] + seq_cache[k-11] - seq_cache[k-12];
        }
        seq_cached_max = n;
    }
    return seq_cache[n-1];
}
