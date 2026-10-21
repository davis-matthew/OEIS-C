/*
 * A074662  a(n) = Fibonacci(n+1)+cos(n*Pi/2).
 * https://oeis.org/A074662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-3) + a(n-4)
 * Initial values: a(0)=2, a(1)=1, a(2)=1, a(3)=3
 * This matches the OEIS definition
 * a(n) = Fibonacci(n+1) + cos(n*Pi/2)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static idx_t   count  = 0;   /* number of terms already initialized */
    static idx_t   capacity = 0; /* allocated capacity */

    /* enlarge cache if necessary */
    if (n >= capacity) {
        idx_t newcap = capacity ? capacity * 2 : 4;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        capacity = newcap;
    }

    /* compute missing values */
    for (idx_t i = count; i <= n; ++i) {
        if (i == 0)      cache[i] = (seq_t)2;
        else if (i == 1) cache[i] = (seq_t)1;
        else if (i == 2) cache[i] = (seq_t)1;
        else if (i == 3) cache[i] = (seq_t)3;
        else             cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }
    count = n + 1;   /* we now have terms 0 … n */

    return cache[n];
}
