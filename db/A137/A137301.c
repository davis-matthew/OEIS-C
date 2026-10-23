/*
 * A137301  Decimal expansion of 999/9801.
 * https://oeis.org/A137301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of the fraction 999/9801 (i.e. the
 *            digit obtained at step n of the long division of 999 by 9801 in
 *            base 10)
 * Range:     u8 none, u16 n <= 2, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t NUM = (seq_t)999;
static const seq_t DEN = (seq_t)9801;
static seq_t rem = NUM;
static seq_t *cache = NULL;     /* cached digits */
static size_t cap = 0;          /* size of cache */

seq_t a(idx_t n)
{
    /* Extend the cache as needed (cache[0] corresponds to a(0)) */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory exhaustion */
        cache = tmp;
        /* Compute missing digits from cap up to n */
        for (idx_t i = cap; i <= n; ++i) {
            rem *= (seq_t)10;          /* bring down next digit */
            seq_t digit = rem / DEN;   /* integer division gives the digit */
            rem  %= DEN;               /* update remainder */
            cache[i] = digit;
        }
        cap = new_cap;
    }
    return cache[n];
}
