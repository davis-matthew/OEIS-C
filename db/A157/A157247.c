/*
 * A157247  Positive numbers y such that y^2 is of the form x^2+(x+2401)^2
 *          with integer x.
 * https://oeis.org/A157247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A157247
   Formula: a(n) = 6*a(n-9) - a(n-18) for n > 18,
   Initial values: a(1)=1715, a(2)=1781, a(3)=1855, a(4)=2009, a(5)=2401,
   a(6)=2989, a(7)=3451, a(8)=3821, a(9)=4459,
   a(10)=5831, a(11)=6865, a(12)=7679, a(13)=9065,
   a(14)=12005, a(15)=15925, a(16)=18851, a(17)=21145,
   a(18)=25039
*/

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[18] = {
    (seq_t)1715, (seq_t)1781, (seq_t)1855, (seq_t)2009, (seq_t)2401,
    (seq_t)2989, (seq_t)3451, (seq_t)3821, (seq_t)4459,
    (seq_t)5831, (seq_t)6865, (seq_t)7679, (seq_t)9065,
    (seq_t)12005, (seq_t)15925, (seq_t)18851, (seq_t)21145, (seq_t)25039
};

seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is 1‑indexed; value for n==0 unused */
        return (seq_t)0;

    /* Static cache of computed terms */
    static seq_t *cache   = NULL;   /* cache[1]..cache[last] initialized */
    static size_t cap     = 0;      /* current capacity of cache array */
    static idx_t last     = 0;      /* highest n for which cache[n] is valid */

    /* Grow cache if necessary */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed – stop execution */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 18) {
            cache[i] = init_vals[i-1];
        } else {
            cache[i] = cache[i-9] * (seq_t)6 - cache[i-18];
        }
    }
    if (last < n) last = n;

    return cache[n];
}
