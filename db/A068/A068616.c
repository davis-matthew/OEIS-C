/*
 * A068616  Starting from a(1)=7, each subsequent term is the minimal square
 *          obtained by inserting at least one digit into the previous term.
 * https://oeis.org/A068616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1)=7, a(2)=576, a(3)=5476, a(4)=54756, a(n)=100*a(n-1) for n>4
 *            (equivalently a(n)=1547536*100^(n-5) for n≥5)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;
    if (n == 2)
        return (seq_t)576;
    if (n == 3)
        return (seq_t)5476;
    if (n == 4)
        return (seq_t)54756;

    /* n >= 5 */
    const seq_t base = (seq_t)1547536;      /* a(5) */
    idx_t exp = (n - 5);
    exp = exp + exp;                        /* exp = 2*(n-5)  = log10(100^(n-5)) */
    seq_t pow = pow10(exp);                 /* 10^exp = 100^(n-5) */
    return base * pow;
}
