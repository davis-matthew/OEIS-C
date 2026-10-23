/*
 * A140147  a(1)=1, a(n)=a(n-1)+n^1 if n odd, a(n)=a(n-1)+ n^5 if n is even.
 * https://oeis.org/A140147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=a(n-1)+n^1 if n odd, a(n)=a(n-1)+n^5 if n even
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the values in a dynamically
   growing 1‑based array.  The array is expanded only when the
   requested index exceeds the current maximum.  All operations
   are performed on seq_t directly, avoiding any unnecessary
   casts.  The implementation works for any integer size that
   seq_t may represent (8, 16, 32, 64, 128 bits or arbitrary
   precision). */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* 0‑based indices are not defined – return 0 for safety. */
    if (n == 0) return 0;

    /* Static cache: 1‑based array.  cache[0] is unused. */
    static seq_t *cache = NULL;   /* pointer to the array */
    static size_t  max   = 0;     /* largest index that is currently computed */

    /* First call – allocate space for index 1 and store a(1). */
    if (cache == NULL) {
        /* Allocate room for indices 0 and 1 – array length 2. */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 0;                  /* unused */
        cache[1] = 1;                  /* a(1) = 1 */
        max = 1;
    }

    /* If the requested index is not yet computed, grow the array. */
    if ((size_t)n > max) {
        /* New array size (including unused index 0) */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        /* Compute the missing values in increasing order. */
        for (size_t i = max + 1; i <= (size_t)n; ++i) {
            seq_t inc;
            if (i & 1U) {                  /* odd */
                inc = (seq_t)i;
            } else {                       /* even: i^5 */
                seq_t sq  = (seq_t)i * (seq_t)i;          /* i^2 */
                seq_t cu  = sq * (seq_t)i;                /* i^3 */
                seq_t qt  = cu * (seq_t)i;                /* i^4 */
                inc = qt * (seq_t)i;                      /* i^5 */
            }
            cache[i] = cache[i - 1] + inc;
        }
        max = (size_t)n;
    }

    return cache[(size_t)n];
}
