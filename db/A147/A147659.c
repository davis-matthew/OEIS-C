/*
 * A147659  Expansion of 1/(1-x-x^2-x^10+x^12).
 * https://oeis.org/A147659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A147659: coefficients of 1/(1-x-x²-x¹⁰+x¹²)
 *  Recurrence:  a(0)=1,
 *               a(n)=a(n-1)+a(n-2)+a(n-10)-a(n-12)  for n≥1,
 *  with a(k)=0 for k<0.
 *  The function memoises results so calls may come in any order.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoisation array */
    static seq_t *cache = NULL;     /* pointer to stored terms */
    static idx_t cap    = 0;        /* allocated capacity */
    static idx_t len    = 0;        /* number of terms currently computed */

    /* initial allocation if needed */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) {
            /* allocation failed – abort */
            abort();
        }
        cache[0] = (seq_t)1;       /* a(0)=1 */
        len = 1;
    }

    /* grow the array if we need more space */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            abort();
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            seq_t val = cache[i-1];          /* a(i-1) */
            if (i >= 2)      val += cache[i-2];   /* a(i-2) */
            if (i >= 10)     val += cache[i-10];  /* a(i-10) */
            if (i >= 12)     val -= cache[i-12];  /* a(i-12) */
            cache[i] = val;
        }
        len = n + 1;
    }

    return cache[n];
}
