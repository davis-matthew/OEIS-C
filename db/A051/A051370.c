/*
 * A051370  a(n+1) = a(n) + sum of digits of a(n)^2, with a(1) = 8.
 * https://oeis.org/A051370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 8
   a(n+1) = a(n) + digit_sum( a(n)^2 )      (n ≥ 1)
   digit_sum(x) = sum of decimal digits of x
   Note: a(n) is stored (and returned) in type seq_t.
*/
#include <stdlib.h>

static seq_t digit_sum_square(seq_t v)
{
    seq_t sq = v * v;   /* compute a(n)^2 */
    seq_t sum = 0;
    while (sq > 0) {
        sum += sq % 10;
        sq  /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety – not part of the OEIS range */

    static seq_t *cache = NULL;  /* a[0] = a(1) */
    static idx_t  cap   = 0;     /* allocated size */
    static idx_t  last  = 0;     /* number of terms currently cached */

    /* initialise cache with first term */
    if (!cache) {
        cap  = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* a(1) = 8 */
        cache[0] = 8;
        last    = 1;
    }

    /* extend cache if needed */
    if (n > last) {
        if (n > cap) {
            idx_t newcap = cap;
            while (newcap < n) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failure – unlikely */
            cache = tmp;
            cap   = newcap;
        }
        for (idx_t i = last; i < n; ++i) {
            seq_t prev  = cache[i - 1];          /* a(i)   */
            seq_t inc   = digit_sum_square(prev);/* digit sum of a(i)^2 */
            cache[i]    = prev + inc;            /* a(i+1)  */
        }
        last = n;
    }
    return cache[n - 1];
}
