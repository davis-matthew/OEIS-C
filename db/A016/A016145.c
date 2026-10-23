/*
 * A016145  Expansion of g.f. 1/((1-3*x)*(1-10*x)).
 * https://oeis.org/A016145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow10, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016145: a(n) = (10^(n+1) - 3^(n+1)) / 7
   Recurrence used for small seq_t types: a(0)=1, a(n)=10*a(n-1)+3^n.
   For large seq_t types (arbitrary-precision) the closed form is evaluated
   directly using pow10 and pow3 from utils. */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the simple 64‑bit (or smaller) case with a recurrence that
       avoids overflow of the intermediate 10*a and the 3^n term. */
    if (sizeof(seq_t) <= 8) {
        /* For n > 19 the value would not fit in 64 bits anyway, so
           the loop is only needed up to the point where the result
           stays within 64 bits.  The caller is not expected to test
           values beyond that limit for the 64‑bit build. */
        unsigned long long result = 1ULL;      /* a(0) */
        if (n == 0) return (seq_t)result;

        unsigned long long three_pow = 3ULL;   /* 3^1 */
        for (idx_t i = 1; i <= n; ++i) {
            /* 10 * a(i-1) fits in 128 bits when result <= 1.844674e19.
               Using __int128 for the temporary multiplication keeps the
               value correct for all required n. */
            unsigned long long mul10 = (unsigned long long)((__int128)result * 10);

            if (i > 1) {
                /* 3^i grows as 3^i = 3 * 3^(i-1); the product fits in
                   64 bits for i <= 20, which is the largest n that
                   keeps a(i) within 64 bits. */
                three_pow = (unsigned long long)((__int128)three_pow * 3);
            }

            result = mul10 + three_pow;
        }
        return (seq_t)result;
    }

    /* For arbitrary‑precision seq_t the closed form using big integers
       is safe and fast.  pow10 and pow3 are guaranteed to use the
       current seq_t type. */
    seq_t p10 = pow10(n + 1);
    seq_t p3  = pow3(n + 1);
    seq_t diff = p10 - p3;
    return diff / (seq_t)7;
}
