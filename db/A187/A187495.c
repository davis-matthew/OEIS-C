/*
 * A187495  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-3,0,1,2}, n=3*r+p_i, and define a(-3)=1. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,1,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(2*cos(Pi/9)).
 * https://oeis.org/A187495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + 3*a(n-6) - 2*a(n-9) - a(n-12), * initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0, * a(6)=0,
 *            a(7)=1, a(8)=0, a(9)=2, a(10)=0, a(11)=1
 * Range:     u8 n <= 36, u16 n <= 63, u32 n <= 116, u64 n <= 221,
 *            u128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define A187495_MAX 20000  /* sufficient for n up to about 10000 */

static seq_t cache[A187495_MAX];
static idx_t initialized = 0;
static idx_t computed_upto = 0;

seq_t a(idx_t n)
{
    /* initialise cache only once */
    if (!initialized) {
        /* set the first 12 terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)0;
        cache[7] = (seq_t)1;
        cache[8] = (seq_t)0;
        cache[9] = (seq_t)2;
        cache[10] = (seq_t)0;
        cache[11] = (seq_t)1;
        computed_upto = 11;
        initialized = 1;
    }

    /* compute missing terms up to n */
    if (n > computed_upto) {
        size_t start = (size_t)computed_upto + 1;
        size_t limit = (size_t)n;
        if (limit >= (size_t)A187495_MAX) {
            /* out of bounds - truncate to maximum */
            limit = A187495_MAX - 1;
        }
        for (size_t i = start; i <= limit; ++i) {
            /* recurrence: a(n) = a(n-3) + 3*a(n-6) - 2*a(n-9) - a(n-12) */
            cache[i] = cache[i - 3]
                       + ( (seq_t)3 * cache[i - 6] )
                       - ( (seq_t)2 * cache[i - 9] )
                       - cache[i - 12];
        }
        computed_upto = (idx_t)limit;
    }

    /* if n is beyond our array, just return last computed value */
    if (n >= (idx_t)A187495_MAX)
        return cache[ computed_upto ];

    return cache[n];
}
