/*
 * A136377  Write down numbers from 0 to 19 around a circle. Start at 1, jump
 *          7; repeat; when you land on a number it is removed.
 * https://oeis.org/A136377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Josephus permutation of 0..19 with step 7; a(n) = order[n mod 20] */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The elimination order for 20 elements with step 7 is fixed.
       Compute it once lazily and reuse it for all calls. */
    static seq_t order[20];
    static int init = 0;
    if (!init) {
        seq_t values[20];
        idx_t sz = 20;
        for (idx_t i = 0; i < sz; ++i) {
            values[i] = (seq_t)i;
        }
        /* Start removal at number 1 (index 1). */
        idx_t pos = 1;
        idx_t step = 0;
        while (step < 20) {
            order[step++] = values[pos];
            /* Remove the element at pos by shifting left. */
            for (idx_t i = pos; i < sz - 1; ++i) {
                values[i] = values[i + 1];
            }
            sz--;
            if (sz == 0) {
                break;
            }
            if (pos >= sz) {
                pos = 0;
            }
            /* Advance by (step size - 1) = 6 positions. */
            pos = (pos + 6) % sz;
        }
        init = 1;
    }
    return order[n % (idx_t)20];
}
