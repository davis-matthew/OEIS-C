/*
 * A103194  LAH transform of squares.
 * https://oeis.org/A103194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   (n-1)*a(n) = n*(2*n-1)*a(n-1) - n*(n-1)^2*a(n-2), initial values
 *            a(0)=0, a(1)=1. The function uses memoisation and 128-bit
 *            intermediate arithmetic to avoid overflow when seq_t is a 64‑bit
 *            unsigned integer
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* memoised a(0)…a(k) */
static idx_t  cached = 0;     /* number of stored values */

/* Return the n-th term of OEIS sequence A103194. */
seq_t a(idx_t n)
{
    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache = seq_malloc(2);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cached = 2;                     /* we have a[0] and a[1] */
    }

    /* extend cache if necessary */
    if (n + 1 > cached) {
        seq_t *tmp = seq_realloc(cache, n + 1);
        if (!tmp) return 0;             /* out‑of‑memory */
        cache = tmp;

        for (idx_t i = cached; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                /* compute using 128‑bit intermediates to avoid overflow
                   in 64‑bit seq_t builds. */
                unsigned __int128 t1 = (unsigned __int128)cache[i - 1] * (2*i - 1);
                unsigned __int128 t2 = (unsigned __int128)cache[i - 2] *
                                      (i - 1) * (i - 1);
                unsigned __int128 t  = t1 - t2;
                unsigned __int128 num = (unsigned __int128)i * t;
                cache[i] = (seq_t)(num / (i - 1));
            }
        }
        cached = n + 1;                  /* cache now holds a[0] … a[n] */
    }

    return cache[n];
}
