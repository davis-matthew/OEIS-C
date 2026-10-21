/*
 * A050075  a(n) = |a(n-1) - a(m)| for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A050075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050075: a(1)=1, a(2)=2, a(3)=3;
 * for n>=4: a(n)=|a(n-1)−a(m)| where
 * p = ⌊log2(n‑2)⌋,  m = (n‑1) – 2^p  (p is the largest integer < log2(n‑1) ).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[1]…a[n]; computed_len = last computed index */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t computed = 0;     /* highest index computed so far */

    /* Ensure the cache is large enough */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? (cap << 1) : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise the first three terms if needed */
    if (computed < 1) { cache[1] = 1; computed = 1; }
    if (computed < 2) { cache[2] = 2; computed = 2; }
    if (computed < 3) { cache[3] = 3; computed = 3; }

    /* Compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* For k >= 4 only */
        if (k >= 4) {
            /* p = floor(log2(k-2))  (largest power of two < k-1) */
            idx_t temp = k - 2;
            idx_t p = 0;
            while (temp >>= 1) ++p;
            idx_t power = ((idx_t)1) << p;
            idx_t m = (k - 1) - power;          /* 1 <= m < k */

            seq_t prev = cache[k - 1];
            seq_t val_m = cache[m];
            cache[k] = (prev > val_m) ? (prev - val_m) : (val_m - prev);
        } else {
            /* k is 1,2,3 – already initialised */
        }
    }
    computed = (computed > n) ? computed : n;

    return cache[n];
}
