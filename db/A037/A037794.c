/*
 * A037794  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,1,0.
 * https://oeis.org/A037794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3; for n>1, a(n) = a(n-1)*5 + p[(n-1) mod 4] with
 *            p=[3,2,1,0]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑indexed cache of previously computed terms */
static size_t cache_size = 0;    /* number of terms stored in cache (>=1 if cache != NULL) */

static const seq_t pattern[4] = {3, 2, 1, 0};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe return */

    /* ensure cache is large enough */
    if (cache == NULL) {
        /* allocate space for terms 1..n (index 0 unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!cache) exit(1);           /* handle allocation failure */
        cache[1] = (seq_t)3;           /* a(1) */
        cache_size = 1;
    } else if ((size_t)n > cache_size) {
        /* grow cache to hold up to n */
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) exit(1);
        cache = tmp;
        /* compute new terms */
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t digit = pattern[(i - 1) & 3];  /* (i-1) mod 4 */
            cache[i] = prev * (seq_t)5 + digit;
        }
        cache_size = (size_t)n;
    }

    return cache[n];
}
