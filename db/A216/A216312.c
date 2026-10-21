/*
 * A216312  The prime ending in 9 is the only prime in a decade.
 * https://oeis.org/A216312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 727, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A216312
    a(n) = the nth prime p in the arithmetic progression 10k+9
    such that 10k+1, 10k+3 and 10k+7 are all composite.
    The function merely tests candidates sequentially.  It
    does *not* rely on any particular arithmetic properties
    beyond the definition. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0, but return 0 safely */
        return 0;

    idx_t   found   = 0;
    uint64_t m      = 1;

    for (;; ++m) {
        uint64_t cand = 10 * m + 9;          /* candidate = 10k+9 */

        if (!isPrimeGeneric((seq_t)cand))
            continue;                       /* not prime */

        /* Check the three preceding numbers in the progression. */
        if (isPrimeGeneric((seq_t)(cand - 8))  /* 10k+1 */
         || isPrimeGeneric((seq_t)(cand - 6))  /* 10k+3 */
         || isPrimeGeneric((seq_t)(cand - 2)))/* 10k+7 */
            continue;                       /* one of them is prime */

        ++found;
        if (found == n)
            return (seq_t)cand;
    }
}
