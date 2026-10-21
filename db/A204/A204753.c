/*
 * A204753  Number of (n+2) X 9 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n>5 with a(1)=1051, a(2)=70,
 *            a(3)=69, a(4)=74, a(5)=82 (empirical recurrence from OEIS
 *            A204753)
 * Range:     u8 none, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; we return 0 for n==0 for safety */
    if (n == 0) return (seq_t)0;

    /* static cache storing computed values a(1)..a(cache_size) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* if we need more values than currently stored, expand */
    if (cache_size < n)
    {
        idx_t new_size = n;
        /* realloc includes initial allocation (realloc(NULL, …) is valid) */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) abort();  /* out of memory */

        /* ensure the first five values are present */
        if (cache_size < 5)
        {
            idx_t init_end = (5 < new_size ? 5 : new_size);
            /* fill missing initial entries */
            for (idx_t i = cache_size; i < init_end; ++i)
            {
                if (i == 0) cache[i] = (seq_t)1051;
                else if (i == 1) cache[i] = (seq_t)70;
                else if (i == 2) cache[i] = (seq_t)69;
                else if (i == 3) cache[i] = (seq_t)74;
                else if (i == 4) cache[i] = (seq_t)82;
            }
            cache_size = init_end;
        }

        /* compute remaining values using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            /* a(i+1) = 2*a(i) - a(i-2)   (indices are 0‑based) */
            cache[i] = (cache[i - 1] * (seq_t)2) - cache[i - 3];
        }
        cache_size = new_size;
    }

    return cache[n - 1];  /* a(1) is stored at cache[0] */
}
