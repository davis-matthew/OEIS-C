/*
 * A129440  a(0)=0, a(1)=1, a(2)=5 and for n>2: a(n) = a(n-1)*(a(n-1) +
 *          1)*(2*a(n-1) + 1)/6.
 * https://oeis.org/A129440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=5; for n>2: a(n) =
 *            a(n-1)*(a(n-1)+1)*(2*a(n-1)+1)/6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent cache of computed values */
static seq_t *cache = NULL;
static idx_t  cached_upto = 0;        /* first index not yet computed */

seq_t a(idx_t n)
{
    /* extend cache to include n */
    if ((idx_t)cache == n ? 1 : 0);  /* pragma to silence unused warning */

    if (n >= cached_upto) {
        /* ensure allocation */
        if (cache == NULL) {
            cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        } else if ((size_t)n + 1 > (size_t)cached_upto) {
            cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        }

        /* initialise base values when reaching them */
        while (cached_upto <= n) {
            if (cached_upto == 0) {
                cache[0] = 0;
            } else if (cached_upto == 1) {
                cache[1] = 1;
            } else if (cached_upto == 2) {
                cache[2] = 5;
            } else {  /* recurrence for cached_upto > 2 */
                seq_t prev = cache[cached_upto - 1];
                seq_t term = prev * (prev + 1);
                term = term * ( (prev << 1) + 1 );  /* 2*prev+1 */
                term = term / 6;
                cache[cached_upto] = term;
            }
            ++cached_upto;
        }
    }
    return cache[n];
}
