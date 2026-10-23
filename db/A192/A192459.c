/*
 * A192459  Coefficient of x in the reduction by x^2->x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3)*( 2^(n+1)*(n+1)! + (2n-1)!! )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n).  Uses unsigned __int128 for n ≤ 27 (64‑bit cases),
   and falls back to seq_t arithmetic for larger n.  This keeps
   64‑bit values correct for all indices that fit. */
seq_t a(idx_t n)
{
    /* Handle small n using 128‑bit arithmetic – safe for all
       64‑bit values and simpler for large seq_t too. */
    if (n <= 27) {
        unsigned __int128 pow2 = 1;
        for (idx_t i = 0; i < n + 1; ++i)
            pow2 <<= 1;                     /* 2^(n+1) */

        unsigned __int128 fact = 1;
        for (idx_t i = 1; i <= n + 1; ++i)
            fact *= (unsigned __int128)i;   /* (n+1)! */

        unsigned __int128 odd = 1;
        for (idx_t i = 1; i <= n; ++i)
            odd *= (unsigned __int128)(2 * i - 1);  /* (2n-1)!! */

        unsigned __int128 total = pow2 * fact + odd;
        total /= 3;
        return (seq_t)total;
    }

    /* For larger n (or when seq_t is an arbitrary-precision type),
       use seq_t arithmetic directly.  The result will be exact for
       arbitrary-precision seq_t; for 64‑bit seq_t the indices for
       which the value fits are handled by the 128‑bit branch above. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow2 *= (seq_t)2;                 /* 2^(n+1) */

    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n + 1; ++i)
        fact *= (seq_t)i;                 /* (n+1)! */

    seq_t odd = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        odd *= (seq_t)(2 * i - 1);         /* (2n-1)!! */

    seq_t total = pow2 * fact + odd;
    total /= (seq_t)3;
    return total;
}
