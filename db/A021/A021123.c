/*
 * A021123  Decimal expansion of 1/119.
 * https://oeis.org/A021123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence a(n): digit n of the decimal expansion of 1/119 (offset 0).  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t digits[48];

    if (!initialized) {
        idx_t r = 1;                      // current remainder from 1/119
        for (idx_t i = 0; i < 48; ++i) {  // 119 has period 48
            idx_t q = (r * 10) / 119;     // next digit
            digits[i] = (seq_t)q;
            r = (r * 10) % 119;           // update remainder
        }
        initialized = true;
    }

    idx_t pos = n % 48;                    // repeat cycle
    return digits[pos];
}
