/*
 * A101979  Antidiagonal sums of A101309, which is the matrix logarithm of
 *          A047999 (Pascal's triangle mod 2).
 * https://oeis.org/A101979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n≥2, if n is even: a(n)=a(n/2)+a(n/2-1); if
 *            n is odd: a(n)=1 if n≡1 (mod 4), otherwise 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* static cache for memoised values */
static seq_t *CACHE = NULL;
static idx_t CACHE_SIZE = 0;      /* number of elements in CACHE */
static idx_t MAX_COMPUTED = 0;    /* last index that has been computed */

/* compute a(n) */
seq_t a(idx_t n) {
    /* ensure the cache can hold indices up to n */
    if (n + 1 > CACHE_SIZE) {
        idx_t new_size = n + 1;
        if (CACHE == NULL) {
            CACHE = seq_malloc(new_size);
            CACHE_SIZE = new_size;
        } else {
            CACHE = seq_realloc(CACHE, new_size);
            CACHE_SIZE = new_size;
        }
    }

    /* initialise the very first element if not yet done */
    if (MAX_COMPUTED == 0) {
        CACHE[0] = (seq_t)0;
    }

    /* compute any missing values up to n */
    for (idx_t i = MAX_COMPUTED + 1; i <= n; ++i) {
        if (i & 1) {                  /* odd index */
            CACHE[i] = ((i & 3) == 1) ? (seq_t)1 : (seq_t)0;
        } else {                      /* even index */
            idx_t half = i >> 1;     /* i / 2 */
            CACHE[i] = CACHE[half] + CACHE[half - 1];
        }
    }
    MAX_COMPUTED = n;
    return CACHE[n];
}
