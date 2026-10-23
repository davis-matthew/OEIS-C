/*
 * A140832  a(0) = 1; if a(n) is even, a(n + 1) = 5a(n)/2; if a(n) is odd, a(n
 *          + 1) is 5a(n)/2 rounded to the nearest even integer.
 * https://oeis.org/A140832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   if a(n) even   -> a(n+1) = 5*a(n)/2
   if a(n) odd    -> a(n+1) = 5*a(n)/2 rounded to the nearest even integer   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last = 0;   /* highest index computed so far */

    if (!cache) {            /* first call: initialize cache */
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;
        last = 0;
    }

    if (n <= last)           /* already computed */
        return cache[n];

    /* Extend cache to hold up to index n */
    cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;

        if ((prev & 1) == 0) {          /* even case: 5*prev/2 */
            next = (prev >> 1) * 5;
        } else {                        /* odd case */
            seq_t half = prev >> 1;     /* floor(prev/2) */
            seq_t base = half * 5;      /* 5*floor(prev/2) */
            seq_t floor = base + 2;     /* floor of 5*prev/2 */
            next = (floor & 1) ? floor + 1 : floor; /* nearest even */
        }

        cache[i] = next;
    }

    last = n;
    return cache[n];
}
