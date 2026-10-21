/*
 * A051406  a(n) = (3^n+1) * (3^(n+1)+1) / 8.
 * https://oeis.org/A051406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n+1) * (3^(n+1)+1) / 8 (OEIS A051406)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* calloc, realloc, free, malloc (C++) */

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* cache of computed values */
    static idx_t len = 0;       /* number of terms stored */
    static size_t cap = 0;      /* allocated capacity (size_t for malloc) */

    /* initialise cache with first three terms */
    if (len == 0) {
        /* start with a small capacity */
        cap = 4;
#ifdef __cplusplus
        /* C++: allocate with new[] which default‑constructs the elements */
        buf = new seq_t[cap];
#else
        /* C: allocate with calloc (seq_t is an integer type) */
        buf = (seq_t *)calloc(cap, sizeof(seq_t));
#endif
        if (!buf) return 0;                     /* allocation error */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)5;
        buf[2] = (seq_t)35;
        len = 3;
    }

    /* return cached value if already computed */
    if (n < len) return buf[n];

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 4;
        while (new_cap <= (size_t)n) new_cap = new_cap * 2 + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < len; ++i) tmp[i] = buf[i];
        delete[] buf;
        buf = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation error */
        buf = tmp;
#endif
        cap = new_cap;
    }

    /* compute missing terms up to n using the 3‑term recurrence */
    for (idx_t i = len; i <= n; ++i) {
        buf[i] = (seq_t)(13 * buf[i-1] - 39 * buf[i-2] + 27 * buf[i-3]);
    }
    len = n + 1;

    return buf[n];
}
