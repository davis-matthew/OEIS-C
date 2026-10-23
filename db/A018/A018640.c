/*
 * A018640  Divisors of 776.
 * https://oeis.org/A018640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: All divisors of 776 in increasing order */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t divisors[8];
    static bool initialized = false;
    if (!initialized) {
        /* 2^0 .. 2^3 */
        seq_t val = 1;
        for (idx_t e = 0; e < 4; ++e) {
            divisors[e] = val;
            val *= 2;
        }
        /* 97 * 2^0 .. 97 * 2^3 */
        val = 97;
        for (idx_t e = 0; e < 4; ++e) {
            divisors[4 + e] = val;
            val *= 2;
        }
        initialized = true;
    }

    if (n <= 8) return divisors[n - 1];
    return 0; /* out of range */
}
