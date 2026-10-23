/*
 * A102561  a(n) = 2^floor(n/2)*((-1)^floor(n/2) + (-1)^n)/2.
 * https://oeis.org/A102561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^floor(n/2)*((-1)^floor(n/2) + (-1)^n)/2
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2)                                     */
    seq_t val = 1;             /* holds 2^k during the following loop          */

    if ((n & 1u) == 0) {       /* even n                                          */
        if (k & 1u)            /* k odd → term is 0                               */
            return 0;
        /* compute 2^k                                            */
        for (idx_t i = 0; i < k; ++i)
            val <<= 1;
        return val;
    } else {                   /* odd n                                            */
        if ((k & 1u) == 0)     /* k even → term is 0                              */
            return 0;
        /* compute 2^k and negate                                  */
        for (idx_t i = 0; i < k; ++i)
            val <<= 1;
        return -val;
    }
}
