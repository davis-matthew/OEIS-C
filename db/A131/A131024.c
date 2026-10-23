/*
 * A131024  Row sums of triangular array T: T(j,1) = 1 for ((j-1) mod 6) < 3,
 *          else 0; T(j,k) = T(j-1,k-1) + T(j-1,k) for 2 <= k <= j.
 * https://oeis.org/A131024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n‑1) – 14*a(n‑2) + 14*a(n‑3) – 14*a(n‑5) + 15*a(n‑6)
 *            – 6*a(n‑7) Initial values: a(1)=1, a(2)=3, a(3)=7, a(4)=11,
 *            a(5)=16, a(6)=22, a(7)=36 (derived from the generating function
 *            of OEIS A131024)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t base_vals[8] = {0, 1, 3, 7, 11, 16, 22, 36};

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;   /* not defined for offset < 1 */

    static seq_t *cache  = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_last  = 0;

    /* initialise cache with the first 7 values on first use */
    if (cache == NULL) {
        cache_size = 8;               /* indices 0 … 7 */
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i) cache[i] = base_vals[i];
        cache_last = 7;
    } else if (n + 1 > cache_size) {
        /* grow the cache if a larger n is requested */
        idx_t new_size = n + 4; /* a little overshoot for future calls */
        seq_t *newc = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        cache = newc;
        cache_size = new_size;
    }

    if (n <= cache_last)
        return cache[n];

    /* recurrence holds for n ≥ 8; cache_last is at least 7 */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* compute a(i) using the recurrence with only additions and a final subtraction */
        seq_t pos = cache[i-1] * 6;
        pos += cache[i-3] * 14;
        pos += cache[i-6] * 15;

        seq_t sub = cache[i-2] * 14;
        sub += cache[i-5] * 14;
        sub += cache[i-7] * 6;

        cache[i] = pos - sub;  /* pos ≥ sub, so no underflow for unsigned types */
    }

    cache_last = n;
    return cache[n];
}
