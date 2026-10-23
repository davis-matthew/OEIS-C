/*
 * A022876  a(n) = [ a(n-1)/a(1) ] + [ a(n-2)/a(2) ] + ... + [ a(1)/a(n-1) ],
 *          for n >= 3.
 * https://oeis.org/A022876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=1;
   For n >= 3, a(n) = Σ_{k=1}^{n-1} ⌊ a(n-k) / a(k) ⌋ */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for previously computed terms; zero-indexed (a(1) at index 0) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;       /* number of terms currently stored */

    if (n == 0) return (seq_t)0;  /* off‑by‑one safety, not used by OEIS offset */

    /* initialize cache with first two terms on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – return zero */
        cache[0] = (seq_t)2;         /* a(1) */
        cache[1] = (seq_t)1;         /* a(2) */
        cap = 2;
    }

    /* extend cache if necessary */
    if (n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return zero */
        cache = tmp;
        for (idx_t cur = cap + 1; cur <= n; ++cur) {
            seq_t sum = (seq_t)0;
            /* compute sum_{k=1}^{cur-1} floor( a(cur-k) / a(k) ) */
            for (idx_t k = 1; k < cur; ++k) {
                sum += cache[cur - 1 - k] / cache[k - 1];
            }
            cache[cur - 1] = sum;
        }
        cap = n;
    }

    return cache[n - 1];
}
