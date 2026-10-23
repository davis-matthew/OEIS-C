/*
 * A177143  Pasquale's sequence: a(n) = 2a(n-1) + (-1)^n*floor(n/2), with
 *          a(1)=1.
 * https://oeis.org/A177143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + (-1)^n*floor(n/2), a(1)=1 (OEIS A177143 –
 *            Pasquale's sequence)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* highest index that holds a value */

    if (n == 0) return 0;        /* undefined for n==0, return 0 */

    /* Allocate or enlarge cache to hold indices up to n */
    if (cache_max < n) {
        idx_t new_len = n + 1;    /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed */
        cache = tmp;

        /* Initialise new slots */
        idx_t init_start = (cache_max == 0) ? 2 : cache_max + 1;
        if (cache_max == 0) {    /* first time: set a(1) */
            cache[1] = (seq_t)1;
        }
        for (idx_t i = init_start; i < new_len; ++i) {
            cache[i] = 0;
        }
        cache_max = n;
    }

    /* Compute any missing entries up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;  /* already known */
        seq_t prev  = cache[i - 1];
        seq_t value = prev * 2;       /* 2*a(i-1) */

        if (i & 1) {                 /* odd i */
            value -= (seq_t)((i - 1) / 2);
        } else {                     /* even i */
            value += (seq_t)(i / 2);
        }
        cache[i] = value;
    }

    return cache[n];
}
