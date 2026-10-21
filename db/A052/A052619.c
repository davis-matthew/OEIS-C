/*
 * A052619  Expansion of e.g.f. 3*x^3/(1-x).
 * https://oeis.org/A052619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n ≤ 2; a(n) = 3 * n! for n > 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache factorials: fact[i] = i! for i ≥ 0 */
    static seq_t *fact = NULL;
    static idx_t max_cached = 0;   /* next unused index in fact */

    if (n < 3)
        return (seq_t)0;

    /* initialise cache with 0!, 1!, 2! */
    if (max_cached == 0) {
        fact = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!fact) exit(1);          /* out of memory */
        fact[0] = (seq_t)1;          /* 0! = 1 */
        fact[1] = (seq_t)1;          /* 1! = 1 */
        fact[2] = (seq_t)2;          /* 2! = 2 */
        max_cached = 3;
    }

    /* extend cache to at least (n + 1) factorials */
    if (n + 1 > max_cached) {
        size_t new_cap = (size_t)(n + 1);
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!fact) exit(1);          /* out of memory */
        for (idx_t i = max_cached; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        max_cached = n + 1;
    }

    /* return 3 * n! */
    return fact[n] * (seq_t)3;
}
