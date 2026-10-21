/*
 * A224703  Numbers divisible by the twice the number of their prime factors
 *          (counted with multiplicity), or numbers n divisible by 2*Omega(n).
 * https://oeis.org/A224703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 6773, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A224703: numbers n such that n % (2*Ω(n)) == 0. */
#include <stdint.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not defined, safe return */

    idx_t seen = 0;
    idx_t x = 1;                       /* start checking from 2 */
    while (seen < n)
    {
        ++x;
        idx_t omega = (idx_t)bigomega(x);     /* Ω(x) as unsigned integer */
        if (omega == 0)                        /* skip the case x==1 */
            continue;
        seq_t divisor = (seq_t)(2 * omega);    /* 2*Ω(x) */
        seq_t val      = (seq_t)x;
        if (val % divisor == 0)                /* condition satisfied */
        {
            ++seen;
            if (seen == n)
                return val;
        }
    }
    /* unreachable – the loop always returns */
    return 0;
}
