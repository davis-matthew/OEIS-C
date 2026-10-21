/*
 * A201640  a(n) = Sum_{k=1..n} k*binomial(n, k)^3*(n^2 + n - k*n - k +
 *          k^2)/((n - k + 1)^2*n).
 * https://oeis.org/A201640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A201640 by its 4‑term recurrence.
   Formula (recurrence):
   (n+1)^2*a(n) = n*(13*n+2)*a(n-1) - 6*(2*n-1)*(3*n-5)*a(n-2)
                  - 2*(17*n^2-62*n+32)*a(n-3) + 16*(n-4)*(n-3)*a(n-4).
   Initial values: a(1)=1, a(2)=8, a(3)=54, a(4)=368. */

#include <stdint.h>
#include <limits.h>

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    typedef __int128   signed_big_t;
    typedef unsigned __int128 unsigned_big_t;
#else
    /* Fallback: use seq_t as the intermediate type.  The expression
       always fits into seq_t in the 64‑bit build because the result
       itself fits in seq_t when it is requested. */
    typedef seq_t signed_big_t;
    typedef seq_t unsigned_big_t;
#endif

/* Safe conversion macros */
#define BIG_FROM_SEQ(x)  ((signed_big_t)(x))
#define BIG_TO_SEQ(x)    ((seq_t)(x))

seq_t a(idx_t n)
{
    /* 0-index not used in OEIS; return 0 for safety. */
    if (n == 0) return 0;

    /* Initial values a(1)..a(4). */
    const seq_t a1 = (seq_t)1;
    const seq_t a2 = (seq_t)8;
    const seq_t a3 = (seq_t)54;
    const seq_t a4 = (seq_t)368;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* Sliding window of the last four terms.  prev1 = a(n-1),
       prev2 = a(n-2), prev3 = a(n-3), prev4 = a(n-4). */
    seq_t prev1 = a4;
    seq_t prev2 = a3;
    seq_t prev3 = a2;
    seq_t prev4 = a1;
    seq_t cur   = a4; /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        /* Convert loop index to seq_t for arithmetic. */
        seq_t tn = (seq_t)i;

        /* Compute each term of the numerator using a big intermediate. */
        signed_big_t term1 = BIG_FROM_SEQ(tn) *
                             BIG_FROM_SEQ((seq_t)(13 * tn + 2)) *
                             BIG_FROM_SEQ(prev1);

        signed_big_t term2 = BIG_FROM_SEQ((seq_t)6) *
                             BIG_FROM_SEQ((seq_t)(2 * tn - 1)) *
                             BIG_FROM_SEQ((seq_t)(3 * tn - 5)) *
                             BIG_FROM_SEQ(prev2);

        signed_big_t term3 = BIG_FROM_SEQ((seq_t)2) *
                             BIG_FROM_SEQ((seq_t)(17 * tn * tn - 62 * tn + 32)) *
                             BIG_FROM_SEQ(prev3);

        signed_big_t term4 = BIG_FROM_SEQ((seq_t)16) *
                             BIG_FROM_SEQ((seq_t)(tn - 4)) *
                             BIG_FROM_SEQ((seq_t)(tn - 3)) *
                             BIG_FROM_SEQ(prev4);

        signed_big_t num = term1 - term2 - term3 + term4;
        signed_big_t den = BIG_FROM_SEQ((seq_t)(tn + 1)) *
                           BIG_FROM_SEQ((seq_t)(tn + 1));

        cur = BIG_TO_SEQ(num / den);

        /* Shift the sliding window for next iteration. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
