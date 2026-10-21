/*
 * A099271  Unsigned member r=-13 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1)+14*a(n-2)-a(n-3) Initial values: a(0)=0,
 *            a(1)=1, a(2)=13
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache up to 20000 terms – sufficient for required n≤10000 */
    static seq_t cache[20001];
    static idx_t next = 0;          /* first index not yet initialized */

    /* initialise the first three terms once */
    if (next == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        next = 3;
    }

    /* if already computed, return directly */
    if (n < next)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t temp = ((seq_t)14) * cache[i-1] + ((seq_t)14) * cache[i-2];
        cache[i] = temp - cache[i-3];
    }

    next = n + 1;   /* update cache limit */
    return cache[n];
}
