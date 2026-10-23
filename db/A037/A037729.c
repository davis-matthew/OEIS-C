/*
 * A037729  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,0,3,1.
 * https://oeis.org/A037729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the decimal number formed by the first n terms of the
 *            periodic sequence 2,0,3,1,2,0,3,1,…. Recurrence: a(1)=2;
 *            a(n)=a(n-1)*10 + digit(n) where digit(n) = 2,0,3,1 repeated with
 *            period 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoization storage */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* last n for which cache is filled */
    static idx_t capacity = 0;   /* allocated size of cache */

    if (n == 0)
        return 0;              /* undefined for offset 1, just return 0 */

    /* allocate or grow cache to at least n+1 elements (index 0 unused) */
    if (cache == NULL) {
        capacity = n + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 0;
        cached_n = 0;
    } else if (capacity < n + 1) {
        capacity = n + 1;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* compute missing values up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t digit;
        switch ((i - 1) % 4) {
            case 0: digit = 2; break;
            case 1: digit = 0; break;
            case 2: digit = 3; break;
            case 3: digit = 1; break;
            default: digit = 0; /* unreachable */
        }
        if (i == 1)
            cache[i] = digit;
        else
            cache[i] = cache[i - 1] * 10 + digit;
    }
    cached_n = n;
    return cache[n];
}
