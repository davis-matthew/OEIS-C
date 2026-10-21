/*
 * A237341  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(4).
 * https://oeis.org/A237341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0; for n ≥ 0, a(n+1) = a(n) + 1, unless a(n) ends in 4,
 *            in which case a(n+1) = (a(n)/10)*100 + 16
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = 0;
    for (idx_t i = 0; i < n; ++i) {
        if ((cur % 10) == 4) {
            cur = (cur / 10) * 100 + 16;
        } else {
            ++cur;
        }
    }
    return cur;
}
