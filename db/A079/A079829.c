/*
 * A079829  a(n) = smallest k such that floor(reverse(k)/k) >= n.
 * https://oeis.org/A079829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest k such that floor(reverse(k)/k) >= n.
   The sequence A079829 is defined only for n <= 9; for larger n it
   is undefined.  This implementation searches k from 1 upward until
   the condition is met. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 0 || n > 9) /* sequence is undefined for n > 9 */
        return (seq_t)0;

    seq_t k = 1;
    for (;;) {
        seq_t rev = 0;
        seq_t t = k;
        /* reverse the decimal digits of k */
        while (t > 0) {
            rev = rev * 10 + t % 10;
            t /= 10;
        }
        if (rev / k >= n)
            return k;
        ++k;
    }
}
