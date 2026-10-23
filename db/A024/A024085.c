/*
 * A024085  a(n) = 7^n - n^10.
 * https://oeis.org/A024085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *pow7 = NULL;   /* 7^k cache */
    static idx_t cache_len = 0;  /* number of elements in cache */

    /* initialize cache for n=0 */
    if (pow7 == NULL) {
        cache_len = 1;
        pow7 = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!pow7) return 0;          /* allocation failure (won’t happen in tests) */
        pow7[0] = (seq_t)1;           /* 7^0 */
    }

    /* extend cache until it contains 7^n */
    while (cache_len <= n) {
        seq_t next = pow7[cache_len - 1] * (seq_t)7;   /* 7^(k) = 7^(k-1)*7 */
        seq_t *tmp = (seq_t*)realloc(pow7, sizeof(seq_t) * (cache_len + 1));
        if (!tmp) return 0;                     /* allocation failure */
        pow7 = tmp;
        pow7[cache_len] = next;
        cache_len++;
    }

    /* compute n^10 */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < 10; ++i)
        pow10 *= (seq_t)n;

    /* return 7^n - n^10 */
    return pow7[n] - pow10;
}
