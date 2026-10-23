/*
 * A108896  Numbers whose outer two digits are 9's and inner digits are 4's.
 * https://oeis.org/A108896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2); a(1) = 949; a(2) = 9449
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety */
    if (n < 1) return (seq_t)0;

    if (n == 1) return (seq_t)949;
    if (n == 2) return (seq_t)9449;

    seq_t a1 = (seq_t)949;   /* a(1) */
    seq_t a2 = (seq_t)9449;  /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = 11*a(n-1) - 10*a(n-2) */
        next = a2 * (seq_t)11 - a1 * (seq_t)10;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
