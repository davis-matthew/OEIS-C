/*
 * A162692  Strictly positive numbers k such that 28*k/(28+k) are integers.
 * https://oeis.org/A162692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth positive integer k such that 28·k/(28 + k) is an
 *            integer
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t j = 1; j <= 27; ++j) {
        seq_t num = (seq_t)28 * j;
        seq_t den = (seq_t)28 - j;
        if (num % den == 0) {
            ++cnt;
            if (cnt == n) {
                return num / den;
            }
        }
    }
    return 0;  /* n larger than the length of the finite sequence */
}
