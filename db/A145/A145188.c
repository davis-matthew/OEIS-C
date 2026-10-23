/*
 * A145188  Continued cotangent recurrence a(n+1)=a(n)^3+3*a(n) and a(1)=14.
 * https://oeis.org/A145188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^3 + 3*a(n), a(1) = 14
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (continued cotangent recurrence) */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n‑th term of OEIS sequence A145188.
   idx_t and seq_t are supplied by the build system.
   The function is memoised and works correctly for any call order. */
seq_t a(idx_t n)
{
    /* static cache: indices 1..cache_len are stored */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms cached */

    if (n == 0)
        return (seq_t)0;          /* not part of the sequence; defined for safety */

    /* if the requested index is beyond the current cache, extend it */
    if (n > cache_len) {
        /* allocate/re‑allocate enough space (index 0 unused) */
        size_t new_len = (size_t)(n + 1);   /* n ≤ 10000, always fits in size_t */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure → terminate */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)14;
            } else {
                seq_t prev = cache[i - 1];
                /* a(i) = prev^3 + 3*prev */
                cache[i] = prev * prev * prev + (seq_t)3 * prev;
            }
        }
        cache_len = n;
    }

    return cache[n];
}
