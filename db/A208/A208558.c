/*
 * A208558  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208558: number of 6×n 0..1 arrays avoiding
   horizontally 0 0 0 and 0 0 1, and vertically 0 0 1 and 0 1 1.
   Recurrence:
       a(1)=16, a(2)=256, a(3)=784
       a(n)=4*a(n‑1)+12*a(n‑2)–27*a(n‑3)   (n≥4) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices start at 1 */
    static seq_t *cache       = NULL;  /* array of terms */
    static idx_t  cache_size  = 0;    /* allocated size (>= n+1) */
    static idx_t  computed    = 0;    /* highest index computed so far */

    if (n == 0) return 0;  /* not used for OEIS offset 1, safety only */

    /* Allocate or grow the cache if needed */
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;          /* at least n+1, double for amortised O(n) */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failure: return 0 for safety */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)16;
        } else if (i == 2) {
            cache[i] = (seq_t)256;
        } else if (i == 3) {
            cache[i] = (seq_t)784;
        } else {
            seq_t term = 4 * cache[i - 1];      /* 4*a(n-1) */
            term += 12 * cache[i - 2];          /* +12*a(n-2) */
            term -= 27 * cache[i - 3];          /* –27*a(n-3) */
            cache[i] = term;
        }
    }
    computed = n;

    return cache[n];
}
