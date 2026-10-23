/*
 * A199595  Numbers n such that 3n+6, 3n+7 and 3n+8 are all composite.
 * https://oeis.org/A199595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th integer such that 3(k+5)+7 and 3(k+5)+8 are composite.
   Equivalently, let b(n) be the n-th integer for which 3b+7 and 3b+8 are
   composite; then a(n) = b(n) - 5.  This generates the OEIS sequence
   A199595 with offset 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache for the generated terms (b(n) values) */
    static seq_t *cache     = NULL;   /* array of b(n) values */
    static size_t  cap      = 0;      /* current capacity of cache      */
    static size_t  sz       = 0;      /* number of terms stored         */
    static idx_t   next     = 1;      /* next integer to test          */

    if (n == 0) return 0; /* undefined for n<1 */

    /* Grow the cache if necessary */
    if (n > cap) {
        size_t newcap = cap ? cap << 1 : 64;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = newcap;
    }

    /* Generate terms until size reaches n */
    while (sz < n) {
        seq_t candidate = next;                                 /* k to test   */
        seq_t m1 = candidate * (seq_t)3 + (seq_t)7;              /* 3k+7       */
        seq_t m2 = candidate * (seq_t)3 + (seq_t)8;              /* 3k+8       */

        if (!is_prime_big(m1) && !is_prime_big(m2))
            cache[sz++] = candidate;                           /* store b(n) */

        ++next;
    }

    /* Return a(n) = b(n) - 5 */
    return cache[n - 1] - (seq_t)5;
}
