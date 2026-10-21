/*
 * A090086  Smallest pseudoprime to base n, not necessarily exceeding n (cf.
 *          A007535).
 * https://oeis.org/A090086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LeastComposite{x; n^(x-1) mod x = 1}
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the smallest composite x such that n^(x-1) ≡ 1 (mod x).  */
/*  The search proceeds in increasing order starting at 4.       */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/mod_pow.h"

/* External linkage as required.  */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* base n (as seq_t) */
    for (uint64_t xi = 4; ; ++xi) /* candidate composite x, 64‑bit counter */
    {
        seq_t x = (seq_t)xi;           /* current candidate as seq_t */

        /* skip primes and numbers < 4 */
        if (x < 4 || isPrimeGeneric(x))
            continue;

        /* test the congruence */
        seq_t r = mod_pow(nn, xi - 1, x);   /* n^(x-1) mod x */
        if (r == 1)
            return x;                       /* found the smallest composite */
    }
}
