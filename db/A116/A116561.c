/*
 * A116561  Expansion of g.f. x*(1 +4*x +4*x^2 +18*x^3 +7*x^4 +7*x^5 -2*x^6
 *          -x^7 -x^8 -x^9) / (1-20*x^6+x^12).
 * https://oeis.org/A116561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 20*a(n-6) - a(n-12). Initial terms: * a(0)=0, a(1)=1,
 *            a(2)=4, a(3)=4, a(4)=18, a(5)=7, * a(6)=7, a(7)=18, a(8)=79,
 *            a(9)=79, a(10)=359, a(11)=140
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store values up to the largest n seen so far */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Resize cache if needed */
    if (cache == NULL || n >= cache_size) {
        /* grow cache, doubling until it can hold n */
        idx_t new_size = (cache ? cache_size * 2 : 1);
        while (new_size <= n) new_size <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)            /* allocation failed – just return 0 */
            return (seq_t)0;
        cache = tmp;

        /* initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 12) {        /* known initial values */
                switch (i) {
                case  0: cache[i] = 0;   break;
                case  1: cache[i] = 1;   break;
                case  2: cache[i] = 4;   break;
                case  3: cache[i] = 4;   break;
                case  4: cache[i] = 18;  break;
                case  5: cache[i] = 7;   break;
                case  6: cache[i] = 7;   break;
                case  7: cache[i] = 18;  break;
                case  8: cache[i] = 79;  break;
                case  9: cache[i] = 79;  break;
                case 10: cache[i] = 359; break;
                case 11: cache[i] = 140; break;
                }
            } else {              /* recurrence  a(i)=20*a(i-6)-a(i-12) */
                cache[i] = ((seq_t)20 * cache[i-6]) - cache[i-12];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
