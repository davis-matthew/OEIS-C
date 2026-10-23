/*
 * A021361  Decimal expansion of 1/357.
 * https://oeis.org/A021361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of 1/357 (period 48)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t block[48];
    static bool ready = false;

    if (!ready) {
        idx_t rem = 1;
        for (int i = 0; i < 48; ++i) {
            idx_t prod = rem * 10U;
            block[i] = (seq_t)(prod / 357U);
            rem = prod % 357U;
        }
        ready = true;
    }

    size_t pos = (size_t)(n % 48U);
    return block[pos];
}
