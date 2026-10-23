/*
 * A034717  Dirichlet convolution of Catalan numbers (1,2,5,14...) with
 *          themselves.
 * https://oeis.org/A034717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet convolution of Catalan numbers with themselves.
   a(n) = Σ_{d|n} catalan(d) * catalan(n/d)                      */

#include <stdlib.h>
#include "../utils/catalan.h"

/* Integer square root of n: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;      /* ceil((lo+hi)/2) */
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* External linkage function computing the sequence */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                 /* undefined for n<1, harmless guard */

    seq_t sum = 0;
    idx_t limit = isqrt_u64(n);
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            seq_t prod = catalan(d) * catalan(e);
            sum += prod;
            if (d != e)                  /* add the symmetric divisor pair */
                sum += prod;
        }
    }
    return sum;
}
