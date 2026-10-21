/*
 * A083084  A diagonal of number array A083075.
 * https://oeis.org/A083084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+3)*(2n+9)^n + 1)/(n+4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Compute a(n) for all supported seq_t types.
   For fixed-width integer types the calculation is performed in
   __int128 to prevent overflow of intermediate values.  For builds
   that provide an arbitrary‑precision seq_t, the generic big_pow()
   from the utils library is used. */
seq_t a(idx_t n)
{
    /* Cast the index once to the result type. */
    seq_t sn = (seq_t)n;

    /* 64‑bit (or smaller) builds: use __int128 for intermediate work. */
    if (sizeof(seq_t) <= 8)
    {
        __int128 base = (__int128)2 * n + 9;      /* 2n + 9 */
        __int128 pow  = 1;                        /* (2n+9)^n */

        /* Binary exponentiation using __int128. */
        idx_t exp = n;
        while (exp > 0)
        {
            if (exp & 1)  pow *= base;
            base *= base;
            exp >>= 1;
        }

        __int128 num = (__int128)(n + 3) * pow + 1; /* (n+3)*(2n+9)^n + 1 */
        seq_t res = (seq_t)(num / (n + 4));         /* integer division */

        return res;
    }

    /* Arbitrary‑precision builds: rely on big_pow() from utils. */
    seq_t base   = 2 * sn + 9;        /* 2n + 9  (all in seq_t) */
    seq_t pow_n  = big_pow(base, sn); /* (2n+9)^n  */
    seq_t num    = (sn + 3) * pow_n;  /* (n+3)*(2n+9)^n   */
    num += 1;                         /* +1                */
    seq_t res    = num / (sn + 4);     /* exact division    */

    return res;
}
