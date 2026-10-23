/*
 * A134554  Concatenate the terms of three copies of the Fibonacci sequence.
 * https://oeis.org/A134554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A074842(Fibonacci(n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   digit_len, fib_val, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concat the decimal representation of the n-th Fibonacci number
   three times (leading zeros omitted).  */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/fib_val.h"

seq_t a(idx_t n)
{
    seq_t f = fib_val(n);
    if (f == 0) return 0;           /* F(0) = 0 -> concatenation is 0 */

    /* Number of decimal digits in f (digit_len works for 0 as 1). */
    int d = digit_len(f);
    idx_t e = (idx_t)d;              /* exponent for powers of ten */

    seq_t pow_d   = pow10(e);         /* 10^d        */
    seq_t pow_2d  = pow10(e * 2);     /* 10^(2d)     */

    /* a(n) = f*(10^(2d) + 10^d + 1) = f*10^(2d) + f*10^d + f */
    return f * pow_2d + f * pow_d + f;
}
