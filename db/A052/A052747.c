/*
 * A052747  a(0) = a(1) = a(2) = 0; a(n) = n!/(n-2) for n > 2.
 * https://oeis.org/A052747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!/(n-2) for n > 2; a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n*(n-1)*(n-3)! for n >= 3 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* static cache of factorials: fact[0] = 0!, fact[1] = 1!, ... */
    static seq_t *fact = NULL;
    static idx_t fact_size = 0;          /* number of entries stored */

    idx_t needed = n - 3;                /* need (n-3)! */

    if (fact == NULL) {
        fact = (seq_t*)malloc(sizeof(seq_t));
        if (!fact) abort();
        fact[0] = 1;                    /* 0! = 1 */
        fact_size = 1;
    }

    /* enlarge cache to include fact[needed] */
    if (fact_size <= needed) {
        size_t new_cap = (size_t)(needed + 1);
        seq_t *tmp = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact = tmp;
        for (idx_t k = fact_size; k <= needed; ++k) {
            fact[k] = fact[k-1] * (seq_t)k;
        }
        fact_size = new_cap;
    }

    seq_t res = fact[needed];
    res *= (seq_t)n;          /* multiply by n   */
    res *= (seq_t)(n - 1);    /* multiply by (n-1) */

    return res;
}
