/*
 * A132359  Numbers divisible by the square of their last decimal digit.
 * https://oeis.org/A132359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers divisible by the square of their last decimal digit
   (OEIS A132359).  The algorithm simply scans each natural number in
   ascending order until the n‑th qualifying number is found.  It
   satisfies all requirements without using dynamic allocation. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t found = 0;          /* count of qualifying numbers found   */
    seq_t k = 1;              /* current candidate number            */

    while (true) {
        /* Last decimal digit of k */
        int d = (int)(k % (seq_t)10);

        /* Skip numbers ending with 0 – last digit 0 has square 0, so
           divisibility by 0 is undefined and these numbers are not
           part of the sequence. */
        if (d != 0) {
            int d2 = d * d;                /* square of last digit */
            if (k % (seq_t)d2 == 0) {      /* divisibility test */
                ++found;
                if (found == n)            /* nth term reached */
                    return k;
            }
        }
        ++k;                                /* next candidate    */
    }
}
