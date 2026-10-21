/*
 * A237342  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(5).
 * https://oeis.org/A237342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n≥0,
   if a(n) ends in 5, a(n+1) = (a(n)/10)*100 + 25,
   else a(n+1)=a(n)+1.
   This implements the definition for k=5 without overflow. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cur = 0;
    idx_t i;
    for (i = 0; i < n; ++i) {
        if (cur % 10 == 5)
            cur = (cur / 10) * 100 + 25;
        else
            ++cur;
    }
    return cur;
}
