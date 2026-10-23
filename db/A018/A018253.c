/*
 * A018253  Divisors of 24.
 * https://oeis.org/A018253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 24 (1,2,3,4,6,8,12,24). For n>8 the *
 *            sequence has no defined term, so the function returns 0
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Only 8 divisors exist; compute them once. */
    static seq_t divs[8];
    static bool ready = false;
    if (!ready) {
        idx_t idx = 0;
        seq_t p2 = 1;
        for (idx_t a = 0; a <= 3; ++a) {            /* 2^a for a=0..3 */
            seq_t p3 = 1;
            for (idx_t b = 0; b <= 1; ++b) {        /* 3^b for b=0..1 */
                divs[idx++] = p2 * p3;
                p3 *= 3;
            }
            p2 *= 2;
        }
        /* Sort the 8 values in ascending order. */
        for (idx_t i = 0; i < 7; ++i) {
            for (idx_t j = i + 1; j < 8; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        ready = true;
    }
    if (n >= 1 && n <= 8)
        return divs[n - 1];
    else
        return (seq_t)0;
}
