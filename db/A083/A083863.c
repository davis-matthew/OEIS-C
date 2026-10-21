/*
 * A083863  Consider recurrence b(0) = n/3, b(n) = b(n-1)*floor(b(n-1));
 *          sequence gives first integer reached, or -1 if no integer is ever
 *          reached.
 * https://oeis.org/A083863
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first integer reached by the recurrence b0 = n/3 , bk+1 =
 *            bk * floor(bk); a(n) is the first integer bk, or -1 if the
 *            sequence never becomes an integer. The recurrence is computed
 *            until the numerator is divisible by 3
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for indices below the official offset (6). */
    if (n < 6)
        return (seq_t)-1;

    /* The recurrence keeps the denominator fixed at 3.  We store
       only the numerator as a seq_t value. */
    seq_t num = (seq_t)n;            /* b0 numerator */
    const seq_t denom = (seq_t)3;    /* fixed denominator */
    const seq_t seq_max = (seq_t)-1; /* maximum representable value */

    for (;;) {
        /* If the current value is already an integer, we are done. */
        if ((num % denom) == 0)
            return num / denom;

        /* floor(bk) = floor(num/3) */
        seq_t fl = num / denom;

        /* fl must be positive for n ≥ 6; guard against the unlikely case  */
        if (fl == 0)
            return (seq_t)-1;

        /* For builds where seq_t is 8 bytes or smaller we must check
           for overflow before the multiplication.  For arbitrary‑precision
           types the check evaluates to false and is ignored.  */
        if (sizeof(seq_t) <= 8 &&
            fl > 0 &&
            num > seq_max / fl) {
            /* Multiplication would overflow the type – treat as failure. */
            return (seq_t)-1;
        }

        /* Compute the next numerator */
        num = num * fl;
    }
}
