/*
 * A067713  a(n+1) is the smallest square > a(n) such that the digits of a(n)
 *          are all (with multiplicity) contained in the digits of a(n+1),
 *          with a(0)=1.
 * https://oeis.org/A067713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   digit_frequencies, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A067713
    a(n) = smallest square > a(n‑1) with digits of a(n‑1) all contained in its digits
    a(0) = 1
    This implementation caches results so that calls in arbitrary order are efficient.
    It uses a vector when compiled as C++ and a malloc/realloc array for C.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/digit_frequencies.h"

#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* cache for C */
    static seq_t *cache = 0;
    static size_t cache_size = 0;

#if defined(__cplusplus)
    /* cache for C++ */
    static std::vector<seq_t> vec;
#endif

    /* enlarge cache up to index n */
    if ((size_t)n >= cache_size) {
        /* remember previous size */
        size_t prev = cache_size;

#if defined(__cplusplus)
        vec.resize((size_t)n + 1);
        cache_size = vec.size();
#else
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
#endif

        /* compute missing terms */
        for (size_t i = prev; i <= (size_t)n; ++i) {
            if (i == 0) {
#if defined(__cplusplus)
                vec[0] = (seq_t)1;
#else
                cache[0] = (seq_t)1;
#endif
                continue;
            }

            seq_t prev_val;
#if defined(__cplusplus)
            prev_val = vec[i - 1];
#else
            prev_val = cache[i - 1];
#endif

            unsigned int prev_freq[10], cur_freq[10];
            digit_frequencies(prev_val, prev_freq);

            seq_t m = floor_sqrt(prev_val) + (seq_t)1;
            while (true) {
                seq_t cand = m * m;
                digit_frequencies(cand, cur_freq);

                bool ok = true;
                for (int d = 0; d < 10; ++d) {
                    if (prev_freq[d] > cur_freq[d]) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
#if defined(__cplusplus)
                    vec[i] = cand;
#else
                    cache[i] = cand;
#endif
                    break;
                }
                ++m;
            }
        }
    }

#if defined(__cplusplus)
    return vec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
