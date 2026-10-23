/*
 * A143056  a(n) = Re(b(n)) where b(n)=(1+i)*b(n-1)+b(n-2), with b(1)=0,
 *          b(2)=1.
 * https://oeis.org/A143056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) - a(n-4); initial values: a(1)=0,
 *            a(2)=1, a(3)=1, a(4)=1
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 42, i64 n <= 84, i128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the n-th term (n >= 1).  Values are stored in a static cache
   so subsequent calls are handled in O(1) after the first pass up to n. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1, but harmless */

    static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
    static idx_t cached = 0;      /* highest n already computed */

    if (n > cached) {             /* need to extend the cache */
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();              /* unrecoverable memory error */
        cache = tmp;

        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = 0;
            else if (i == 2 || i == 3 || i == 4)
                val = 1;
            else
                val = 2 * cache[i - 2]      /* a(i-1) */
                    - 2 * cache[i - 4]      /* a(i-3) */
                    - cache[i - 5];          /* a(i-4) */
            cache[i - 1] = val;            /* store a(i) */
        }
        cached = n;
    }

    return cache[n - 1];
}
