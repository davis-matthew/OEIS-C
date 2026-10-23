/*
 * A041722  Numerators of continued fraction convergents to sqrt(381).
 * https://oeis.org/A041722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2030*a(n-6) - a(n-12)      for n > 11
 * with initial values
 *   a(0)=19, a(1)=20, a(2)=39, a(3)=488, a(4)=527,
 *   a(5)=1015, a(6)=39097, a(7)=40112, a(8)=79209,
 *   a(9)=990620, a(10)=1069829, a(11)=2060449
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  size   = 0;   /* current allocated slots */
    static idx_t  used   = 0;   /* number of entries already computed */

    /* initialise cache and first 12 terms on first call */
    if (used == 0) {
        size = 12 > n ? 12 : n + 1;
        cache = (seq_t *)realloc(NULL, size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0 ] = 19;
        cache[1 ] = 20;
        cache[2 ] = 39;
        cache[3 ] = 488;
        cache[4 ] = 527;
        cache[5 ] = 1015;
        cache[6 ] = 39097;
        cache[7 ] = 40112;
        cache[8 ] = 79209;
        cache[9 ] = 990620;
        cache[10] = 1069829;
        cache[11] = 2060449;
        used = 12;
    }

    /* enlarge cache if necessary */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        size = new_size;
    }

    /* compute missing terms */
    while (used <= n) {
        if (used < 12) {
            /* values already initialised above */
        } else {
            cache[used] = 2030 * cache[used - 6] - cache[used - 12];
        }
        used++;
    }

    return cache[n];
}
