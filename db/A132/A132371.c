/*
 * A132371  a(n) = n! - Sum_{j=1..n-1} j!.
 * https://oeis.org/A132371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - Sum_{j=1..n-1} j!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* return value for n < 1 is undefined, but handled safely */
    if (n < 1) return (seq_t)0;

    static seq_t *fact = NULL;   /* factorials, fact[0] = 1 */
    static seq_t *cum  = NULL;   /* cumulative sums, cum[0] = 0 */
    static idx_t  size = 0;      /* current allocated size (>= 1) */

    /* expand the arrays to at least n+1 entries */
    if (n >= size)
    {
        size_t new_size = (size_t)(n + 1);
        fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
        cum  = (seq_t *)realloc(cum , new_size * sizeof(seq_t));

        /* initialise the first elements if this is the first call */
        if (size == 0)
        {
            fact[0] = (seq_t)1; /* 0! = 1 */
            cum[0]  = (seq_t)0; /* empty sum */
            size    = 1;
        }

        /* compute factorials and cumulative sums from size up to n */
        for (idx_t i = size; i <= n; ++i)
        {
            fact[i] = fact[i-1] * (seq_t)i;   /* i!  */
            cum[i]  = cum[i-1] + fact[i];    /* 1!+2!+...+i! */
        }
        size = (idx_t)new_size;
    }

    /* a(n) = n! - sum_{j=1}^{n-1} j!  ==  fact[n] - cum[n-1] */
    seq_t result = fact[n] - cum[n-1];
    return result;
}
