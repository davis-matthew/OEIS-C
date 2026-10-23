/*
 * A021725  Decimal expansion of 1/721.
 * https://oeis.org/A021725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th decimal digit of 1/721 (period 102)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return the n'th digit in the decimal expansion of 1/721, where a(0)=0. */
seq_t a(idx_t n)
{
    /* The decimal expansion of 1/721 repeats every 102 digits. */
    static seq_t digits[102];
    static bool initialized = false;

    if (!initialized) {
        idx_t rem = 1; /* start with remainder 1/721 */

        for (idx_t i = 0; i < 102; ++i) {
            rem *= 10;                                 /* shift the remainder */
            digits[i] = (seq_t)(rem / 721);             /* next digit */
            rem = rem % 721;                           /* new remainder */
        }
        initialized = true;
    }

    return digits[(size_t)(n % 102)];
}
