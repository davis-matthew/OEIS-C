/*
 * A237346  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(9).
 * https://oeis.org/A237346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 36, u32 n <= 81, u64 n <= 171,
 *            u128 n <= 342, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>=0,
   a(n+1)= a(n)+1  unless a(n) ends in 9,
   in which case a(n+1) = (a(n)/10)*100 + 81   (k=9) */
#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t cur = 0;
    for (idx_t i = 0; i < n; ++i) {
        if (cur % 10 == 9) {
            cur = (cur / 10) * 100 + 81;
        } else {
            cur = cur + 1;
        }
    }
    return cur;
}
