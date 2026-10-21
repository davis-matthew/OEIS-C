/*
 * A225722  Numbers divisible by their last digit cubed, excluding those whose
 *          last digit is 1.
 * https://oeis.org/A225722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225722: numbers n such that last digit d ≠ 0,1
   and n is divisible by d³. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 0;

    while (1) {
        num = num + 1;

        uint8_t last = (uint8_t)(num % 10);
        if (last == 0 || last == 1) continue;

        seq_t d   = (seq_t)last;
        seq_t cube = d * d * d;
        if (num % cube == 0) {
            count++;
            if (count == n) return num;
        }
    }
}
