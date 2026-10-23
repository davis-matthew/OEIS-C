/*
 * A015942  Positive integers n such that n | (2^n + n/2 - 1).
 * https://oeis.org/A015942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A014945(n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: search for the n‑th positive integer t such that
   t | (2^t + t/2 - 1). Only even t with t/2 odd can satisfy the
   congruence, so we iterate over odd m = t/2 and test the condition. */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for index 0 */

    idx_t count = 0;
    for (idx_t m = 1; ; m += 2)   /* m odd → t = 2*m even */
    {
        idx_t t = 2 * m;
        if (t == 2)               /* the only even term < 6 is 2, omitted in the OEIS list */
            continue;

        /* 2^t mod t */
        seq_t pow_mod = mod_pow((seq_t)2, t, (seq_t)t);

        seq_t half = (seq_t)(t >> 1);
        seq_t sum  = pow_mod + half;     /* sum < 2*t for the range of interest */
        if (sum >= (seq_t)t)
            sum -= (seq_t)t;             /* (pow_mod + half) mod t */

        if (sum == 1)                 /* condition satisfied */
        {
            ++count;
            if (count == n)
                return (seq_t)t;        /* t is the requested term */
        }
    }
}
