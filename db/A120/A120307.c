/*
 * A120307  Inverse determinant of n X n matrix M[i,j] = i*j/(i+j-1).
 * https://oeis.org/A120307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, for n ≥ 1 a(n+1) = a(n) * (2n+1) * Catalan(n)² where
 *            Catalan(n+1) = Catalan(n) * 2(2n+1)/(n+2) and Catalan(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    /* static memoisation state */
    static seq_t *cache = NULL;         /* array of computed a(i) */
    static idx_t last  = 0;             /* highest index computed so far */
    static seq_t catalan = 0;           /* Catalan(last) */
    static size_t cap = 0;              /* current allocation size */

    /* Allocate or grow the cache as necessary */
    if (cache == NULL) {
        /* first call */
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */
        cache[0] = 0;                  /* unused */
        cache[1] = (seq_t)1;           /* a(1) = 1 */
        last = 1;
        catalan = (seq_t)1;            /* Catalan(1) = 1 */
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* If already computed, just return it */
    if (n <= last) return cache[n];

    /* Compute successive values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute multiplier: (2*(i-1)+1) * Catalan(i-1)² */
        seq_t r = catalan;
        r = r * r;                                              /* Catalan² */
        r = r * (seq_t)((2 * (i - 1) + 1));                      /* * (2n+1) */

        cache[i] = cache[i - 1] * r;

        /* update Catalan to Cat(i) from Cat(i-1) */
        seq_t tmp = catalan;
        tmp = tmp * (seq_t)((4 * (i - 1) + 2));                   /* * 2(2n+1) */
        tmp = tmp / (seq_t)(i + 1);                              /* / (n+2) */
        catalan = tmp;
    }

    last = n;
    return cache[n];
}
