/*
 * A021885  Decimal expansion of 1/881.
 * https://oeis.org/A021885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th decimal digit of 1/881 (base 10)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t digits[880];
    static bool init = false;

    if (!init) {
        uint64_t rem = 1;
        for (idx_t i = 0; i < 880; ++i) {
            rem *= 10;
            seq_t d = (seq_t)(rem / 881u);   /* digit */
            digits[i] = d;
            rem %= 881u;
        }
        init = true;
    }

    return digits[n % 880u];
}
