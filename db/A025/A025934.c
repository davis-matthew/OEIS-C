/*
 * A025934  Expansion of 1/((1-2*x)*(1-3*x)*(1-5*x)*(1-10*x)).
 * https://oeis.org/A025934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow_int, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025934 – Expansion of 1/((1-2*x)*(1-3*x)*(1-5*x)*(1-10*x))
   Recurrence used: a(0)=1, a(1)=20,
   a(n)=3^(n+1)-2^(n+1)+15*a(n-1)-50*a(n-2)   (n≥2) */

#include <stddef.h>
#include "../utils/pow_int.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache for already computed terms.  The size 'cap' is
       the number of entries stored (maximum index + 1). */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n + 1 > cap) {                      /* need to extend cache */
        cache = seq_realloc(cache, cap, n + 1);

        /* Compute all new entries one by one. */
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)20;
            } else {
                /* powers of 3 and 2 for exponent i+1 */
                seq_t p3 = pow_int((seq_t)3, i + 1);
                seq_t p2 = pow_int((seq_t)2, i + 1);

                /* apply the recurrence */
                cache[i] = p3 - p2
                         + (seq_t)15 * cache[i - 1]
                         - (seq_t)50 * cache[i - 2];
            }
        }
        cap = n + 1;                         /* update stored size */
    }

    return cache[n];
}
