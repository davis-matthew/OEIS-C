/*
 * A106367  Number of necklaces with n beads of 5 colors, no 2 adjacent beads
 *          the same color.
 * https://oeis.org/A106367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(n/d) * (4*(-1)^d + 4^d) with a(1) =
 *            5, where phi is Euler's totient function
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 189 (2026-11-03)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* The implementation uses a simple divisor loop and an inline phi calculation
   that does not rely on external utilities. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient.h"

/* --- Power of 4 -------------------------------------------------------- */
static seq_t pow4_uint(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)4;
    return res;
}

/* --- Main sequence function --------------------------------------------- */
seq_t a_wide(idx_t n)
{
    if (n == 1)
        return (seq_t)5;

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            seq_t phi1   = totient(n / d);
            seq_t pow4d  = pow4_uint(d);
            seq_t term   = pow4d;
            if (d & 1) term -= (seq_t)4; else term += (seq_t)4;
            term *= phi1;
            sum += term;

            /* paired divisor d2 = n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t phi2   = totient(d);
                seq_t pow4d2 = pow4_uint(d2);
                seq_t term2  = pow4d2;
                if (d2 & 1) term2 -= (seq_t)4; else term2 += (seq_t)4;
                term2 *= phi2;
                sum += term2;
            }
        }
    }
    return sum / n;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
