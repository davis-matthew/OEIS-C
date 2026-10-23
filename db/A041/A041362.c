/*
 * A041362  Numerators of continued fraction convergents to sqrt(195).
 * https://oeis.org/A041362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=13, a(1)=14;
   for n>=2, a(n) = (n%2==0 ? 26 : 1) * a(n-1) + a(n-2); */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t size  = 0;   /* number of terms currently stored */

    /* return from cache if already computed */
    if (n < size) return cache[n];

    /* initialise cache with first two terms if not already done */
    if (size == 0) {
        cap   = 2;
        cache = (seq_t*)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)13;
        cache[1] = (seq_t)14;
        size    = 2;
    }

    /* extend cache up to the desired index */
    while (size <= n) {
        /* coefficient: 26 for even indices >=2, 1 for odd indices */
        seq_t coef = (size % 2 == 0) ? (seq_t)26 : (seq_t)1;
        seq_t next = coef * cache[size - 1] + cache[size - 2];

        /* grow the array if necessary */
        if (size >= cap) {
            idx_t newcap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        cache[size] = next;
        ++size;
    }
    return cache[n];
}
