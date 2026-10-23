/*
 * A125823  Numbers whose base 7 representation is 4444....4.
 * https://oeis.org/A125823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; a(n)=7*a(n-1)+4 for n>1 */

#include <stdlib.h>

static seq_t cache[10001];      /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;    /* number of elements currently computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard – sequence starts at 1 */

    if (n <= cache_size)                  /* already computed */
        return cache[n-1];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        if (i == 0)                       /* a(1) = 0 */
            cache[0] = 0;
        else
            cache[i] = 7 * cache[i-1] + 4;
    }
    cache_size = n;
    return cache[n-1];
}
