/*
 * A140806  Expansion of x *(1+x) *(x^2+1) *(15*x^4+1) / (
 *          (x^4-2*x^3+2*x^2+2*x+1) *(x^4+2*x^3+2*x^2-2*x+1) ).
 * https://oeis.org/A140806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 20, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = -14 * a(n-4) - a(n-8)  for n ≥ 9
   with initial values a(0)=0, a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=a(7)=a(8)=1
   (a(1) is the first term of the sequence).  This follows from
   the rational generating function
       x*(1+x)*(x^2+1)*(15*x^4+1) / ( (x^4-2*x^3+2*x^2+2*x+1) *(x^4+2*x^3+2*x^2-2*x+1) ).
   The denominator simplifies to x^8+14*x^4+1, giving the above linear
   recurrence. */
#include <stdlib.h>

static const seq_t coeff[9] = {
    0,           /* x^0 */
    1,           /* x^1 */
    1,           /* x^2 */
    1,           /* x^3 */
    1,           /* x^4 */
    15,          /* x^5 */
    15,          /* x^6 */
    15,          /* x^7 */
    15           /* x^8 */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    size_t need = (size_t)n + 1;        /* we need indices 0..n */

    if (need > capacity) {
        size_t newcap = capacity ? capacity << 1 : 16;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialize new space to zero for safety */
        for (size_t i = capacity; i < newcap; ++i)
            cache[i] = (seq_t)0;
        capacity = newcap;
    }

    /* compute missing entries */
    static size_t filled = 0;           /* last computed index + 1 */
    for (size_t i = filled; i < need; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i <= 8) {
            seq_t val = coeff[i];
            if (i >= 4) val -= (seq_t)14 * cache[i - 4];
            if (i >= 8) val -= cache[i - 8];
            cache[i] = val;
        } else {
            cache[i] = -(seq_t)14 * cache[i - 4] - cache[i - 8];
        }
    }
    filled = need;

    return cache[(size_t)n];
}
