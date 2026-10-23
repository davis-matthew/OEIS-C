/*
 * A186458  Number of (n+1) X 6 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) (n > 11) Initial
 *            terms: a(1)=106, a(2)=128, a(3)=270, a(4)=449, a(5)=797,
 *            a(6)=1061, a(7)=1681, a(8)=2285, a(9)=3681, a(10)=5281,
 *            a(11)=8437
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 42, u64 n <= 96, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* Return 0 for unused indices < 1; the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;            /* highest index computed so far  */

    /* Resize cache if necessary */
    if (n > cached) {
        size_t new_len = (size_t)n + 1; /* index 0 is unused */
#if defined(__cplusplus)
        /* In C++, use new[] to construct seq_t objects */
        seq_t *tmp = new seq_t[new_len];
        if (cached > 0) {
            std::copy(cache, cache + cached + 1, tmp);
            delete[] cache;
        }
        cache = tmp;
#else
        /* In C, realloc suffices for POD types */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        cache = tmp;
#endif

        /* Compute missing terms */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i <= 11) {
                /* hard‑coded initial values */
                switch ((int)i) {
                case 1:  cache[i] = (seq_t)106;  break;
                case 2:  cache[i] = (seq_t)128;  break;
                case 3:  cache[i] = (seq_t)270;  break;
                case 4:  cache[i] = (seq_t)449;  break;
                case 5:  cache[i] = (seq_t)797;  break;
                case 6:  cache[i] = (seq_t)1061; break;
                case 7:  cache[i] = (seq_t)1681; break;
                case 8:  cache[i] = (seq_t)2285; break;
                case 9:  cache[i] = (seq_t)3681; break;
                case 10: cache[i] = (seq_t)5281; break;
                case 11: cache[i] = (seq_t)8437; break;
                }
            } else {
                seq_t a1 = cache[i-1] << 1;
                seq_t a2 = cache[i-4] << 1;
                seq_t a3 = cache[i-3] << 1;
                seq_t a4 = cache[i-5];
                cache[i] = (a1 + a2) - (a3 + a4);
            }
        }
        cached = n;
    }

    return cache[n];
}
