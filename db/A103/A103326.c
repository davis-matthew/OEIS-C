/*
 * A103326  a(n) = Fibonacci(5n)/Fibonacci(n).
 * https://oeis.org/A103326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + L(n) * L(3n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where L is the Lucas sequence (A000032)  */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  */
    if (n == 0) return 0;                 /* undefined for n=0 – return 0 */

    seq_t ln  = lucas(n);                  /* L(n)   */
    seq_t l3n = lucas(3 * n);              /* L(3n)  */
    return (seq_t)1 + ln * l3n;            /* a(n) = 1 + L(n)L(3n) */
}
