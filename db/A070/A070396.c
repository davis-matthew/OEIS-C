/*
 * A070396  a(n) = 6^n mod 23.
 * https://oeis.org/A070396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n mod 23, with offset 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t tbl[11];
    static bool init = false;
    if (!init) {
        tbl[0] = (seq_t)1;
        for (int i = 1; i < 11; ++i)
            tbl[i] = (tbl[i-1] * (seq_t)6) % (seq_t)23;
        init = true;
    }
    return tbl[n % 11];
}
