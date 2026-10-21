/*
 * A052669  Expansion of e.g.f. (1-2*x)/(1-3*x-x^2+2*x^3).
 * https://oeis.org/A052669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052669 - recurrence
   a(0) = 1
   a(1) = 1
   a(2) = 8
   a(n) = 3*n*a(n-1) + n*(n-1)*a(n-2) - 2*n*(n-1)*(n-2)*a(n-3)   for n >= 3
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out–of–order calls */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t last  = 0;   /* highest index computed */

    /* enlarge the cache if necessary */
    if (cap <= n) {
        /* minimum capacity needed is 3 to hold a(0)..a(2) */
        idx_t need = n + 1;
        if (need < 3) need = 3;
        if (cache == NULL) {
            cache = (seq_t *)malloc(need * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        }
        /* initial values when just allocating */
        if (cap == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)8;
            last = 2;
        } else if (cap == 1) {
            /* we have only a(0) (unlikely), initialise the rest */
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)8;
            last = 2;
        } else if (cap == 2) {
            /* already have a(0), a(1) only, fill a(2) */
            cache[2] = (seq_t)8;
            last = 2;
        }
        cap = need;
    }

    /* compute missing values up to n */
    while (last < n) {
        idx_t next = last + 1;
        seq_t sn   = (seq_t)next;          /* next  = n  */
        seq_t sn_1 = (seq_t)last;         /* next-1 = n-1 */
        seq_t sn_2 = (seq_t)(last - 1);   /* next-2 = n-2 */

        /* term1 = 3 * n * a(n-1) */
        seq_t term1 = (seq_t)3 * sn * cache[last];

        /* term2 = n * (n-1) * a(n-2) */
        seq_t term2 = sn * sn_1 * cache[last - 1];

        /* term3 = 2 * n * (n-1) * (n-2) * a(n-3) */
        seq_t term3 = (seq_t)2 * sn * sn_1 * sn_2 * cache[last - 2];

        cache[next] = term1 + term2 - term3;
        last = next;
    }

    return cache[n];
}
