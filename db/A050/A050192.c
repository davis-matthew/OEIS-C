/*
 * A050192  a(n)=a(n-1)+a(n-2)-d, where d=a(n/2) if n is even, else d=0; 2
 *          initial terms.
 * https://oeis.org/A050192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 51, u64 n <= 97, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(1)=1, a(2)=0; for n≥3
 * a(n)=a(n-1)+a(n-2)-( (n%2==0) ? a(n/2) : 0 )
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t max_index = 0;          /* largest n that has been computed */

    /* Initialise cache for n=1..2 on first use */
    if (max_index == 0) {
        cache = (seq_t *)realloc(cache, 3 * sizeof(seq_t)); /* indices 0..2 */
        if (!cache) return 0; /* allocation failure – unlikely in contest */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 0;
        max_index = 2;
    }

    /* Return directly if already known */
    if (n <= max_index)
        return cache[n];

    /* Extend cache and fill missing terms up to n */
    size_t new_size = (size_t)(n + 1);                /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;           /* allocation failure – ignore */
    cache = tmp;

    /* Compute terms from (max_index+1) up to n */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t sub = (i & 1) ? 0 : cache[i / 2];      /* a(n/2) if even */
        cache[i] = cache[i - 1] + cache[i - 2] - sub;
    }

    max_index = n;
    return cache[n];
}
