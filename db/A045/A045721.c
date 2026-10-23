/*
 * A045721  a(n) = binomial(3*n+1,n).
 * https://oeis.org/A045721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n+1, n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* 1.  The value is C(3n+1,n).  Compute it multiplicatively
       using the identity
           C(3n+1 , n) = ∏_{i=1}^{n} (2n+1+i)/i
       The intermediate result is kept ≤ the final answer
       by cancelling common factors with the divisor at each step.
       This guarantees that 64‑bit builds produce the exact value
       whenever it fits, and arbitrary‑precision builds also work.  */

    if (n == 0) return 1;

    seq_t result = 1;                /* running product          */
    seq_t base   = (seq_t)(2 * (seq_t)n + 1);  /* 2n+1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t num   = base + i;          /* (2n+1)+i = 2n+1+i        */
        seq_t div   = i;                 /* divisor i                */

        /* cancel common factor between numerator and divisor   */
        seq_t g = gcd(num, div);
        num   /= g;
        div   /= g;

        /* cancel common factor between current result and divisor */
        g = gcd(result, div);
        result /= g;
        div    /= g;                     /* div should now be 1       */

        /* multiply and divide in one step.  Use 128‑bit intermediate
           if available (on GCC/Clang it is *signed* __int128).          */
#if defined(__SIZEOF_INT128__) && __STDC_VERSION__ >= 199901L
        result = (seq_t)(((__uint128_t)result * num) / div);
#else
        result = (seq_t)(((result * num) / (uint64_t)div));
#endif
    }
    return result;
}
