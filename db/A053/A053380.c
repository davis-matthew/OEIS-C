/*
 * A053380  a(n) contains n digits '8' or '9' and is divisible by 2^n.
 * https://oeis.org/A053380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1)=8, recurrence: a(n+1) = a(n) + 10^n * (8 +
 *            parity(a(n)/2^n)) where parity(x) = x mod 2. The algorithm
 *            iteratively builds the sequence
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1)=8 */
    if (n == 0) return 0;               /* undefined for n=0, keep zero */
    seq_t res = 8;                      /* current a(n) */
    seq_t pow10 = (seq_t)10;            /* 10^1 for the first update */

    for (idx_t i = 1; i < n; ++i) {
        /* parity of a(i) / 2^i  ==  (res >> i) & 1  */
        bool bit = ((res >> i) & 1) != 0;

        seq_t digit = bit ? 9 : 8;
        res += pow10 * digit;          /* compute a(i+1) */
        pow10 *= (seq_t)10;            /* advance power of 10 for next step */
    }

    return res;
}
