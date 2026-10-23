/*
 * A032090  "BHK" (reversible, identity, unlabeled) transform of 0,1,1,1...
 * https://oeis.org/A032090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 50, u64 n <= 96, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A032090 - BHK transform of 0,1,1,1...  
    Recurrence (derived from the given generating function):
       a(n) = 2*a(n‑1) + a(n‑2) - 3*a(n‑3) + a(n‑4) - a(n‑5) + a(n‑7)
    for n ≥ 9.
    Initial values (n = 1…8):
       a(1)=0, a(2)=1, a(3)=1, a(4)=1,
       a(5)=2, a(6)=2, a(7)=4, a(8)=5.
    The function caches previously computed values for efficient
    repeated calls in any order.
    (The code is valid C11 and C++17.)  
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* array of a(1)…a(max_computed)  (index 0 unused) */
    static idx_t cache_len = 0;      /* highest n for which a(n) is stored         */
    static idx_t cache_cap = 0;      /* allocated capacity of cache                */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with space for at least the first 8 terms */
        cache_cap = (n > 8u) ? n : 8u;
        cache = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t));
        /* hard‑code the known initial terms */
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 2;
        cache[6] = 2;
        cache[7] = 4;
        cache[8] = 5;
        cache_len = 8u;
    }

    /* if the requested index is already available, return it */
    if (n <= cache_len)
        return cache[n];

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        cache_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed; callers should not expect value */
        cache = tmp;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* indices for the recurrence are all defined because i >= 9 */
        seq_t a1 = cache[i-1];   /* a(i-1)  */
        seq_t a2 = cache[i-2];   /* a(i-2)  */
        seq_t a3 = cache[i-3];   /* a(i-3)  */
        seq_t a4 = cache[i-4];   /* a(i-4)  */
        seq_t a5 = cache[i-5];   /* a(i-5)  */
        seq_t a7 = cache[i-7];   /* a(i-7)  */

        cache[i] = 2*a1 + a2 - 3*a3 + a4 - a5 + a7;
    }
    cache_len = n;
    return cache[n];
}
