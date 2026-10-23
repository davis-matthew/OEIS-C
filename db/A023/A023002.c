/*
 * A023002  Sum of 10th powers.
 * https://oeis.org/A023002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} k^10 (OEIS A023002)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"

/* Compute a(n) = n*(n+1)*(2n+1)*(n^2+n-1)*(3n^6+9n^5+2n^4-11n^3+3n^2+10n-5)/66
 * using exact cancellation to stay within seq_t limits. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Basic linear factors */
    seq_t nn  = (seq_t)n;
    seq_t f1  = nn;                    /* n                */
    seq_t f2  = nn + 1;                /* n+1              */
    seq_t f3  = nn << 1;               /* 2*n              */
    f3        = f3 + 1;                /* 2*n+1            */

    /* Powers of n */
    seq_t n2  = nn * nn;               /* n^2              */
    seq_t n3  = n2 * nn;               /* n^3              */
    seq_t n4  = n3 * nn;               /* n^4              */
    seq_t n5  = n4 * nn;               /* n^5              */
    seq_t n6  = n5 * nn;               /* n^6              */

    /* Compute the big polynomial part */
    seq_t pos = (seq_t)3 * n6
              + (seq_t)9 * n5
              + (seq_t)2 * n4
              + (seq_t)3 * n2
              + (seq_t)10 * nn;

    seq_t neg = (seq_t)11 * n3 + (seq_t)5;
    seq_t f4  = n2 + nn - (seq_t)1;    /* n^2+n-1          */
    seq_t f5  = pos - neg;             /* remaining factor */

    /* Multiply all factors with complete cancellation of 66 */
    seq_t res   = (seq_t)1;
    seq_t denom = (seq_t)66;

    /* Helper macro: gcd and reduction */
#define REDUCE_AND_MUL(f)                                 \
    do                                                     \
    {                                                      \
        seq_t g = seq_gcd((f), denom);                    \
        denom   = denom / g;                              \
        res     = res * ((f) / g);                        \
    } while (0)

    REDUCE_AND_MUL(f1);
    REDUCE_AND_MUL(f2);
    REDUCE_AND_MUL(f3);
    REDUCE_AND_MUL(f4);
    REDUCE_AND_MUL(f5);

    if (denom != (seq_t)1)    /* safety check */
        res = res / denom;

    #undef REDUCE_AND_MUL

    return res;
}
