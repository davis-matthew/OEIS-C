/*
 * A208005  Number of nX6 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n‑1) - 2*a(n‑2) - 170*a(n‑3) + 392*a(n‑4) + 322*a(n‑5)
 *                     - 1512*a(n‑6) + 474*a(n‑7) + 1498*a(n‑8) - 942*a(n‑9)
 *                     - 376*a(n‑10) + 326*a(n‑11) + a(n‑12) - 19*a(n‑13)
 *               for n > 14, with initial values
 *   a(1) = 22, a(2) = 484, a(3) = 1690, a(4) = 4993, a(5) = 24512,
 *   a(6) = 90232, a(7) = 486443, a(8) = 2001968, a(9) = 10846870,
 *   a(10) = 47911153, a(11) = 256232284, a(12) = 1188682252,
 *   a(13) = 6276190547, a(14) = 30137070988.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static cache for memoisation; cache size is number of slots allocated */
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    /* initialise cache with the first 14 terms (index 0 is unused) */
    if (cache_size == 0) {
        cache_size = 15; /* indices 0..14 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure → 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)484;
        cache[3] = (seq_t)1690;
        cache[4] = (seq_t)4993;
        cache[5] = (seq_t)24512;
        cache[6] = (seq_t)90232;
        cache[7] = (seq_t)486443;
        cache[8] = (seq_t)2001968;
        cache[9] = (seq_t)10846870;
        cache[10] = (seq_t)47911153;
        cache[11] = (seq_t)256232284;
        cache[12] = (seq_t)1188682252;
        cache[13] = (seq_t)6276190547;
        cache[14] = (seq_t)30137070988;
    }

    /* return cached value if already computed */
    if (n < cache_size) return cache[n];

    /* grow cache to hold values up to n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* allocation failure → 0 */
    cache = tmp;

    /* compute terms from old_size to n using the recurrence */
    for (idx_t k = old_size; k <= n; ++k) {
        seq_t pos = mul_small(cache[k-1], 9);
        pos += mul_small(cache[k-4], 392);
        pos += mul_small(cache[k-5], 322);
        pos += mul_small(cache[k-7], 474);
        pos += mul_small(cache[k-8], 1498);
        pos += mul_small(cache[k-11], 326);
        pos += cache[k-12];

        seq_t neg = mul_small(cache[k-2], 2);
        neg += mul_small(cache[k-3], 170);
        neg += mul_small(cache[k-6], 1512);
        neg += mul_small(cache[k-9], 942);
        neg += mul_small(cache[k-10], 376);
        neg += mul_small(cache[k-13], 19);

        cache[k] = pos - neg;  /* guaranteed non‑negative */
    }

    return cache[n];
}
