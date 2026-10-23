/*
 * A021905  Decimal expansion of 1/901.
 * https://oeis.org/A021905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of decimal expansion of 1/901 (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t period[208];
    static bool initialized = false;
    if (!initialized) {
        uint64_t rem = 1;
        for (idx_t i = 0; i < 208; ++i) {
            rem *= 10ULL;               /* rem < 901, so this is safe */
            period[i] = rem / 901ULL;   /* digit 0‑9 */
            rem %= 901ULL;              /* new remainder */
        }
        initialized = true;
    }
    return period[n % 208];
}
